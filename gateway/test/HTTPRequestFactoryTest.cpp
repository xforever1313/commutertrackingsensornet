#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <Poco/Net/HTTPRequestHandler.h>

#define private public
#define protected public

#include "CTSNSharedGlobals.py"
#include "gateway/BadClientHTTPRequestHandler.h"
#include "gateway/EmailHTTPRequestHandler.h"
#include "gateway/ErrorHTTPRequestHandler.h"
#include "gateway/HTTPRequestFactory.h"
#include "gateway/LogMessageHTTPRequestHandler.h"
#include "gateway/NotFoundHTTPRequestHandler.h"
#include "gateway/ShutdownHTTPRequestHandler.h"
#include "gateway/TextMessageHTTPRequestHandler.h"
#include "gateway/UartTxHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockEventExecutor.h"
#include "MockMariaDB.h"
#include "MockShutdown.h"
#include "MockUart.h"
#include "Secrets.py"

TEST_GROUP(HTTPRequestFactoryTest) {
    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_uart = new testing::StrictMock<Gateway::MockUart>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_request = new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_shutdown = new testing::StrictMock<Gateway::MockShutdown> ();
        m_uut = new Gateway::HTTPRequestFactory(m_shutdown, m_eventExecutor, m_uart, m_mariadb);

        POINTERS_EQUAL(m_uut->m_shutdown, m_shutdown);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_shutdown;
        delete m_request;
        delete m_mariadb;
        delete m_uart;
        delete m_eventExecutor;
    }
    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockUart> *m_uart;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<Gateway::MockShutdown> *m_shutdown;
    Gateway::HTTPRequestFactory *m_uut;
};

TEST(HTTPRequestFactoryTest, createShutdownTest) {
    m_request->setURI(SHUTDOWN_URI);
    m_request->set("user-agent", USER_AGENT);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<Gateway::ShutdownHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, createUartTxTest) {
    m_request->setURI(UART_TX_URI);
    m_request->set("user-agent", USER_AGENT);

    Gateway::UartTxHTTPRequestHandler* handler = dynamic_cast<Gateway::UartTxHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_uart, m_uart);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createTextMessageTest) {
    m_request->setURI(TEXT_MESSAGE_URI);
    m_request->set("user-agent", USER_AGENT);

    Gateway::TextMessageHTTPRequestHandler* handler = dynamic_cast<Gateway::TextMessageHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createEmailTest) {
    m_request->setURI(EMAIL_URI);
    m_request->set("user-agent", USER_AGENT);

    Gateway::EmailHTTPRequestHandler* handler = dynamic_cast<Gateway::EmailHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createLogMessageTest) {
    m_request->setURI(LOG_MESSAGE_URI);
    m_request->set("user-agent", USER_AGENT);

    Gateway::LogMessageHTTPRequestHandler* handler = dynamic_cast<Gateway::LogMessageHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createErrorMessageTest) {
    m_request->setURI(ERROR_MESSAGE_URI);
    m_request->set("user-agent", USER_AGENT);

    Gateway::ErrorHTTPRequestHandler* handler = dynamic_cast<Gateway::ErrorHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, notFoundTest) {
    m_request->setURI("herpaderp");
    m_request->set("user-agent", USER_AGENT);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<Gateway::NotFoundHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, badClientTestBadUserAgent) {
    m_request->setURI(SHUTDOWN_URI);
    m_request->set("user-agent", "Firefox");

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<Gateway::BadClientHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, badClientTestNoUserAgent) {
    m_request->setURI(SHUTDOWN_URI);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<Gateway::BadClientHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

