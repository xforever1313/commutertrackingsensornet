#include <memory>

#include "UnitTest.h"

#include "EventInterface.h"
#include "gateway/NodeCheckEvent.h"
#include "gateway/NodeCheckHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(NodeCheckHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_nodes = new testing::StrictMock<CTSNCommon::MockNodeContainer>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();

        m_uut = new Gateway::NodeCheckHTTPRequestHandler(m_eventExecutor,
                                                         m_mariadb,
                                                         m_nodes);

        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mariadb;
        delete m_nodes;
        delete m_eventExecutor;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockNodeContainer> *m_nodes;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;

    Gateway::NodeCheckHTTPRequestHandler *m_uut;
};

/////
// Post Tests
/////
TEST(NodeCheckHTTPRequestHandlerTest, postMissingField) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeCheckHTTPRequestHandler::POST_MISSING_FIELD);
    CHECK_EQUAL(m_response->_status,
                Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);

}

TEST(NodeCheckHTTPRequestHandlerTest, postFalse) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::NodeCheckHTTPRequestHandler::CHECK_FORM_DATA << "="
                    << "false";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeCheckHTTPRequestHandler::POST_FALSE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);

}

TEST(NodeCheckHTTPRequestHandlerTest, postTrue) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::NodeCheckHTTPRequestHandler::CHECK_FORM_DATA << "="
                    << "true";

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    // Test to ensure the right event was created.
    Gateway::NodeCheckEvent *checkEvent =
        dynamic_cast<Gateway::NodeCheckEvent*>(event.get());

    CHECK(checkEvent != nullptr);
    POINTERS_EQUAL(checkEvent->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(checkEvent->m_mariadb, m_mariadb);
    POINTERS_EQUAL(checkEvent->m_nodes, m_nodes);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeCheckHTTPRequestHandler::POST_TRUE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);

}

/////
// Get Tests
/////
TEST(NodeCheckHTTPRequestHandlerTest, getTest) {
     m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::NodeCheckHTTPRequestHandler::GET_MESSAGE);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
