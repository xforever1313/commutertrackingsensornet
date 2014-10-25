#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <memory>

#define private public
#define protected public

#include "EventInterface.h"
#include "gateway/Gateway.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/LogEvent.h"
#include "gateway/LogMessageHTTPRequestHandler.h"
#include "gateway/Node.h"
#include "gateway/NodeContainer.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"

TEST_GROUP(LogMessageHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_nodes = new testing::StrictMock<Gateway::MockNodeContainer>();
        m_uut = new Gateway::LogMessageHTTPRequestHandler(m_eventExecutor, m_mariadb, m_nodes);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_nodes;
        delete m_eventExecutor;
        delete m_mariadb;

        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    testing::StrictMock<Gateway::MockNodeContainer> *m_nodes;
    Gateway::LogMessageHTTPRequestHandler *m_uut;
};

/// Missing field Tests
TEST(LogMessageHTTPRequestHandlerTest, postMissingAllFieldsTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::LogMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(LogMessageHTTPRequestHandlerTest, postMissingNodeFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::LogMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(LogMessageHTTPRequestHandlerTest, postMissingMessageFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::LogMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Invalid Node Tests
TEST(LogMessageHTTPRequestHandlerTest, exceptionTest) {
    std::string error = "error";
    std::string badNode = "1abc";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << badNode << "&";
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1";

    EXPECT_CALL(*m_nodes, convertStringToNode(badNode))
        .WillOnce(testing::Throw(std::out_of_range(error)));

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), error);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

///Invaid Message Tests
TEST(LogMessageHTTPRequestHandlerTest, postMessageNotAnIntTest1) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "1abc";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(LogMessageHTTPRequestHandlerTest, postMessageNotAnIntTest2) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "abc";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(LogMessageHTTPRequestHandlerTest, postMessageTooLow) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << Gateway::ErrorNumber::FIRST;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(LogMessageHTTPRequestHandlerTest, postMessageTooHigh) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    Gateway::NodeContainer::numberOfNodes = 5;
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << "1&";
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << Gateway::ErrorNumber::END;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ErrorMessage::INVALID_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

///Post success test
TEST(LogMessageHTTPRequestHandlerTest, postSuccess) {
    Gateway::Node node (1, 0x01);

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::NODE_FORM_DATA << "=" << node.getID() << "&";
    m_request->m_ss << Gateway::LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << Gateway::ErrorNumber::TEST_ERROR;

    EXPECT_CALL(*m_nodes, convertStringToNode(std::to_string(node.getID())))
        .WillOnce(testing::Return(node));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::LogEvent *logEvent = dynamic_cast<Gateway::LogEvent*>(event.get());
    CHECK_EQUAL(logEvent->m_errorNumber, Gateway::ErrorNumber::TEST_ERROR);
    CHECK_EQUAL(logEvent->m_node.getID(), node.getID());
    CHECK_EQUAL(logEvent->m_node.getAddress(), node.getAddress());

    CHECK_EQUAL(m_response->m_response.str(), Gateway::LogMessageHTTPRequestHandler::POST_SUCCESS_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

///Get Tests
TEST(LogMessageHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::LogMessageHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

