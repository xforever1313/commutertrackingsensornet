#include <memory>

#include "UnitTest.h"

#include "MockEventExecutor.h"
#include "MockGPIOController.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockNodeContainer.h"
#include "MockUart.h"
#include "pi_node/BatteryCheckEvent.h"
#include "pi_node/BatteryCheckHTTPRequestHandler.h"


TEST_GROUP(BatteryCheckHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_nodes = new testing::StrictMock<CTSNCommon::MockNodeContainer>();
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_gpio = new testing::StrictMock<CTSNCommon::MockGPIOController>();
        m_uart = new testing::StrictMock<CTSNCommon::MockUart>();
        m_uut = new PiNode::BatteryCheckHTTPRequestHandler(m_nodes, m_eventExecutor, *m_gpio, m_uart);

        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(&m_uut->m_gpio, m_gpio);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_uart;
        delete m_gpio;
        delete m_eventExecutor;
        delete m_nodes;
        delete m_request;
        delete m_response;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    testing::StrictMock<CTSNCommon::MockNodeContainer> *m_nodes;
    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockGPIOController> *m_gpio;
    testing::StrictMock<CTSNCommon::MockUart> *m_uart;
    PiNode::BatteryCheckHTTPRequestHandler *m_uut;
};

TEST(BatteryCheckHTTPRequestHandlerTest, handlePostRequestCheckTrueTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << PiNode::BatteryCheckHTTPRequestHandler::CHECK_FORM_DATA << "=true";

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    PiNode::BatteryCheckEvent *batEvent =
        dynamic_cast<PiNode::BatteryCheckEvent*>(event.get());

    CHECK(batEvent != nullptr);

    POINTERS_EQUAL(batEvent->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(&batEvent->m_gpio, m_gpio);
    POINTERS_EQUAL(batEvent->m_nodes, m_nodes);
    POINTERS_EQUAL(batEvent->m_uart, m_uart);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::BatteryCheckHTTPRequestHandler::POST_SUCCESS);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(BatteryCheckHTTPRequestHandlerTest, handlePostRequestCheckFalseTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << PiNode::BatteryCheckHTTPRequestHandler::CHECK_FORM_DATA << "=false";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::BatteryCheckHTTPRequestHandler::POST_FALSE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(BatteryCheckHTTPRequestHandlerTest, handlePostRequestCheckMissingTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(BatteryCheckHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::BatteryCheckHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
