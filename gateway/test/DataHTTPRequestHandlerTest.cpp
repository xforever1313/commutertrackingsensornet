#include <memory>
#include <string>

#include "UnitTest.h"

#include "gateway/DataEvent.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"

TEST_GROUP(DataHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_node = new Gateway::Node(1, 0x01);

        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_nodes = new testing::StrictMock<Gateway::MockNodeContainer>();

        m_uut = new Gateway::DataHTTPRequestHandler(m_eventExecutor,
                                                    m_mariadb,
                                                    m_nodes);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_nodes;
        delete m_mariadb;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
        delete m_node;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    testing::StrictMock<Gateway::MockNodeContainer> *m_nodes;

    Gateway::Node *m_node;

    Gateway::DataHTTPRequestHandler *m_uut;
};

/////
/// Post Tests
/////
TEST(DataHTTPRequestHandlerTest, postMissingAllFieldsTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DataHTTPRequestHandler::POST_FAILURE_MISSING_FIELD);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(DataHTTPRequestHandlerTest, postMissingNodeFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::DataHTTPRequestHandler::RESULT_TYPE_FORM_DATA << "="
                    << "2";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DataHTTPRequestHandler::POST_FAILURE_MISSING_FIELD);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(DataHTTPRequestHandlerTest, postMissingResultFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::DataHTTPRequestHandler::NODE_FORM_DATA << "="
                    << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DataHTTPRequestHandler::POST_FAILURE_MISSING_FIELD);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Invalid Node Tests
TEST(DataHTTPRequestHandlerTest, postNodeNotAnIntTest1) {
    std::string badString = "1abc";
    std::string error = "error";
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::DataHTTPRequestHandler::NODE_FORM_DATA << "=" << badString << "&";
    m_request->m_ss << Gateway::DataHTTPRequestHandler::RESULT_TYPE_FORM_DATA << "=" << "1";

    EXPECT_CALL(*m_nodes, convertStringToNode(badString))
        .WillOnce(testing::Throw(std::out_of_range(error)));

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), error);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Invalid ResultTypeTests
TEST(DataHTTPRequestHandlerTest, postResultNotAnIntTest1) {
    std::string badString = "1abc";
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::DataHTTPRequestHandler::NODE_FORM_DATA << "="
                    << std::to_string(m_node->getID()) << "&";
    m_request->m_ss << Gateway::DataHTTPRequestHandler::RESULT_TYPE_FORM_DATA 
                    << "=" << badString;

    EXPECT_CALL(*m_nodes, convertStringToNode(std::to_string(m_node->getID())))
        .WillOnce(testing::Return(*m_node));

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::DATA_RESULT_BAD_DATA_TYPE_STRING + badString);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Success Test
TEST(DataHTTPRequestHandlerTest, postSuccess) {
    Gateway::DataResultType type = Gateway::DataResultType::HORSE;
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::DataHTTPRequestHandler::NODE_FORM_DATA << "="
                    << std::to_string(m_node->getID()) << "&";
    m_request->m_ss << Gateway::DataHTTPRequestHandler::RESULT_TYPE_FORM_DATA 
                    << "=" << std::to_string(type);

    EXPECT_CALL(*m_nodes, convertStringToNode(std::to_string(m_node->getID())))
        .WillOnce(testing::Return(*m_node));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::DataEvent *dataEvent = dynamic_cast<Gateway::DataEvent*>(event.get());
    CHECK(dataEvent != nullptr);

    CHECK_EQUAL(dataEvent->m_node.getID(), m_node->getID());
    CHECK_EQUAL(dataEvent->m_type, type);
    POINTERS_EQUAL(dataEvent->m_mariadb, m_mariadb);
}

/////
/// Get Tests
/////
TEST(DataHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DataHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

