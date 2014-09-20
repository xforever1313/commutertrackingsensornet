#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <memory>

#define private public
#define protected public

#include "EventInterface.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorHTTPRequestHandler.h"
#include "gateway/Node.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockMariaDB.h"

TEST_GROUP(ErrorHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>;
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>;
        m_uut = new Gateway::ErrorHTTPRequestHandler(m_eventExecutor, m_mariadb);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_eventExecutor;
        delete m_mariadb;

        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    Gateway::ErrorHTTPRequestHandler *m_uut;
};

/// Missing field Tests
TEST(ErrorHTTPRequestHandlerTest, postMissingAllFieldsTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postMissingNodeFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postMissingMessageFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Invalid Node Tests
TEST(ErrorHTTPRequestHandlerTest, postNodeNotAnIntTest1) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "1abc&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::Node::INVALID_NODE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postNodeNotAnIntTest2) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "abc&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::Node::INVALID_NODE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postNodeTooLow) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "0&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::Node::INVALID_NODE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postNodeTooHigh) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    Gateway::Node::numberOfNodes = 5;
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << Gateway::Node::numberOfNodes + 1 << "&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::Node::INVALID_NODE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

///Invaid Message Tests
TEST(ErrorHTTPRequestHandlerTest, postMessageNotAnIntTest1) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1abc";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postMessageNotAnIntTest2) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "abc";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postMessageTooLow) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << Gateway::ErrorNumber::FIRST;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ErrorHTTPRequestHandlerTest, postMessageTooHigh) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    Gateway::Node::numberOfNodes = 5;
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << Gateway::ErrorNumber::END;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

///Post success test
TEST(ErrorHTTPRequestHandlerTest, postSuccess) {
    const unsigned int nodeNumber = 1;

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::NODE_FORM_DATA << "=" << nodeNumber << "&";
    m_request->m_ss << Gateway::ErrorHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << Gateway::ErrorNumber::TEST_ERROR;

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::ErrorEvent *errorEvent = dynamic_cast<Gateway::ErrorEvent*>(event.get());
    CHECK_EQUAL(errorEvent->m_errorNumber, Gateway::ErrorNumber::TEST_ERROR);
    CHECK_EQUAL(errorEvent->m_node, nodeNumber);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorHTTPRequestHandler::POST_SUCCESS_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

///Get Tests
TEST(ErrorHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

