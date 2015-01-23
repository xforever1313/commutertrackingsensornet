#include <memory>
#include <string>

#include "UnitTest.h"

#include "EventInterface.h"
#include "ctsn_common/XBeeTxEvent.h"
#include "ctsn_common/XBeeTxHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockNodeContainer.h"
#include "MockUart.h"

TEST_GROUP(XBeeTxHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_message = "hello world!";
        m_nodeNumber = 1;

        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>;
        m_uart = new testing::StrictMock<CTSNCommon::MockUart>;
        m_nodes = new testing::StrictMock<CTSNCommon::MockNodeContainer>;
        m_uut = new CTSNCommon::XBeeTxHTTPRequestHandler(m_eventExecutor, m_uart, m_nodes);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_nodes;
        delete m_uart;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    std::string m_message;
    unsigned int m_nodeNumber;

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockUart> *m_uart;
    testing::StrictMock<CTSNCommon::MockNodeContainer> *m_nodes;
    CTSNCommon::XBeeTxHTTPRequestHandler *m_uut;

};

///Post Tests
TEST(XBeeTxHTTPRequestHandlerTest, postSuccessTest) {
    CTSNCommon::Node node(m_nodeNumber, m_nodeNumber);

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << CTSNCommon::XBeeTxHTTPRequestHandler::MESSAGE_FORM_DATA
                    << "=" << m_message << "&";
    m_request->m_ss << CTSNCommon::XBeeTxHTTPRequestHandler::NODE_FORM_DATA
                    << "=" << m_nodeNumber;

    EXPECT_CALL(*m_nodes, convertStringToNode(std::to_string(m_nodeNumber)))
        .WillOnce(testing::Return(node));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    CTSNCommon::XBeeTxEvent *xbeeEvent = dynamic_cast<CTSNCommon::XBeeTxEvent*>(event.get());
    CHECK_EQUAL(xbeeEvent->m_message, m_message);
    POINTERS_EQUAL(xbeeEvent->m_uart, m_uart);
    CHECK_EQUAL(xbeeEvent->m_node.getID(), node.getID());
    CHECK_EQUAL(xbeeEvent->m_node.getAddress(), node.getAddress());

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::XBeeTxHTTPRequestHandler::POST_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(XBeeTxHTTPRequestHandlerTest, postTestMissingFields) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::XBeeTxHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(XBeeTxHTTPRequestHandlerTest, postMissingNodeField) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << CTSNCommon::XBeeTxHTTPRequestHandler::MESSAGE_FORM_DATA
                    << "=" << m_message;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::XBeeTxHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(XBeeTxHTTPRequestHandlerTest, postMissingMessageField) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << CTSNCommon::XBeeTxHTTPRequestHandler::NODE_FORM_DATA
                    << "=" << m_nodeNumber;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::XBeeTxHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(XBeeTxHTTPRequestHandlerTest, postBadNodeTest) {
    std::string badNode = "abc";
    std::string error = "error";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << CTSNCommon::XBeeTxHTTPRequestHandler::MESSAGE_FORM_DATA
                    << "=" << m_message << "&";
    m_request->m_ss << CTSNCommon::XBeeTxHTTPRequestHandler::NODE_FORM_DATA
                    << "=" << badNode;

    EXPECT_CALL(*m_nodes, convertStringToNode(badNode))
        .WillOnce(testing::Throw(std::invalid_argument(error)));

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), error);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}


///Get Test
TEST(XBeeTxHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::XBeeTxHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

