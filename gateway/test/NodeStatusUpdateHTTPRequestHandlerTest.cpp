#include <memory>
#include <string>

#include "UnitTest.h"

#include "gateway/NodeStatusUpdateHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"

TEST_GROUP(NodeStatusUpdateHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_nodes = new testing::StrictMock<Gateway::MockNodeContainer>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();

        m_uut = new Gateway::NodeStatusUpdateHTTPRequestHandler(m_eventExecutor,
                                                                m_nodes,
                                                                m_mariadb);

        m_node = new CTSNCommon::Node(1, 0x01);
        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mariadb;
        delete m_node;
        delete m_nodes;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockNodeContainer> *m_nodes;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;

    CTSNCommon::Node *m_node;
    Gateway::NodeStatusUpdateHTTPRequestHandler *m_uut;
};

/////
// Post Tests
/////
TEST(NodeStatusUpdateHTTPRequestHandlerTest, postMissingAllFieldsTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                Gateway::NodeStatusUpdateHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, 
                Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(NodeStatusUpdateHTTPRequestHandlerTest, postMissingNodeFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::STATUS_FORM_DATA << "="
                    << "2";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeStatusUpdateHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(NodeStatusUpdateHTTPRequestHandlerTest, postMissingStatusFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::NODE_FORM_DATA << "="
                    << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeStatusUpdateHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(NodeStatusUpdateHTTPRequestHandlerTest, postBadNodeDataTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::STATUS_FORM_DATA << "="
                    << "2&";
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::NODE_FORM_DATA << "="
                    << "10000";


    std::string error = "error";
    EXPECT_CALL(*m_nodes, convertStringToNode("10000"))
        .WillOnce(testing::Throw(std::out_of_range(error)));

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), error);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(NodeStatusUpdateHTTPRequestHandlerTest, postBadStatusDataTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::STATUS_FORM_DATA << "="
                    << "0&";
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::NODE_FORM_DATA << "="
                    << "1";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::Node::INVALID_NODE_STATUS + "0");
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Succes test
TEST(NodeStatusUpdateHTTPRequestHandlerTest, postSuccess) {
    CTSNCommon::Node::NodeStatus status = CTSNCommon::Node::NodeStatus::OKAY;
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
  
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::NODE_FORM_DATA << "="
                    << std::to_string(m_node->getID()) << "&";
    m_request->m_ss << Gateway::NodeStatusUpdateHTTPRequestHandler::STATUS_FORM_DATA 
                    << "=" << std::to_string(status);

    EXPECT_CALL(*m_nodes, convertStringToNode(std::to_string(m_node->getID())))
        .WillOnce(testing::Return(*m_node));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::NodeStatusUpdateEvent *NodeStatusevent = 
        dynamic_cast<Gateway::NodeStatusUpdateEvent*>(event.get());
    CHECK(event != nullptr);

    CHECK_EQUAL(NodeStatusevent->m_status, status);
    CHECK_EQUAL(NodeStatusevent->m_nodeID, m_node->getID());
    POINTERS_EQUAL(NodeStatusevent->m_nodes, m_nodes);
    POINTERS_EQUAL(NodeStatusevent->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(NodeStatusevent->m_mariadb, m_mariadb);
    
    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeStatusUpdateHTTPRequestHandler::POST_SUCCESS_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

/////
// Get Tests
/////
TEST(NodeStatusUpdateHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeStatusUpdateHTTPRequestHandler::GET_MESSAGE);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

