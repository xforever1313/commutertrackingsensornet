#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <memory>

#define private public
#define protected public

#include "EventInterface.h"
#include "gateway/UartTxEvent.h"
#include "gateway/UartTxHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockUart.h"

TEST_GROUP(UartTxHTTPRequestHandlerTest) {

    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>;
        m_uart = new testing::StrictMock<CTSNCommon::MockUart>;
        m_uut = new Gateway::UartTxHTTPRequestHandler(m_eventExecutor, m_uart);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_uart;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockUart> *m_uart;
    Gateway::UartTxHTTPRequestHandler *m_uut;
};

TEST(UartTxHTTPRequestHandlerTest, postSuccessTest) {
    const std::string message = "hello world!";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::UartTxHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << message;

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::UartTxEvent *uartEvent = dynamic_cast<Gateway::UartTxEvent*>(event.get());
    CHECK_EQUAL(uartEvent->m_message, message);
    POINTERS_EQUAL(uartEvent->m_uart, m_uart);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::UartTxHTTPRequestHandler::POST_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(UartTxHTTPRequestHandlerTest, postMissingFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(UartTxHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::UartTxHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
