#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <Poco/Net/HTTPRequestHandler.h>

#define private public
#define protected public

#include "CTSNSharedGlobals.py"
#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/SettingsParser.h"
#include "gateway/DatabasePokeHTTPRequestHandler.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "gateway/EmailHTTPRequestHandler.h"
#include "gateway/ErrorHTTPRequestHandler.h"
#include "gateway/HTTPRequestFactory.h"
#include "gateway/LogMessageHTTPRequestHandler.h"
#include "gateway/NodeCheckHTTPRequestHandler.h"
#include "gateway/NodeStatusUpdateHTTPRequestHandler.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "gateway/RootHTTPRequestHandler.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "gateway/TextMessageHTTPRequestHandler.h"
#include "gateway/UartTxHTTPRequestHandler.h"
#include "ctsn_common/XBeeTxHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockEventExecutor.h"
#include "MockHTTPPoster.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"
#include "MockShutdown.h"
#include "MockUart.h"

TEST_GROUP(HTTPRequestFactoryTest) {
    TEST_SETUP() {
        m_settings = &CTSNCommon::Settings::getInstance();
        m_settings->m_stringSettings["GATEWAY_AGENT"] = "someAgent";

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_uart = new testing::StrictMock<CTSNCommon::MockUart>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_request = new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_shutdown = new testing::StrictMock<CTSNCommon::MockShutdown> ();
        m_nodes = new testing::StrictMock<CTSNCommon::MockNodeContainer>();
        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();
        m_uut = new Gateway::HTTPRequestFactory(m_shutdown, m_eventExecutor,
                                                m_uart, m_mariadb, m_nodes,
                                                m_httpPoster);

        POINTERS_EQUAL(m_uut->m_shutdown, m_shutdown);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_httpPoster, m_httpPoster);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_httpPoster;
        delete m_nodes;
        delete m_shutdown;
        delete m_request;
        delete m_mariadb;
        delete m_uart;
        delete m_eventExecutor;
    }

    CTSNCommon::Settings *m_settings;
    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockUart> *m_uart;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<CTSNCommon::MockShutdown> *m_shutdown;
    testing::StrictMock<CTSNCommon::MockNodeContainer> *m_nodes;
    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;
    Gateway::HTTPRequestFactory *m_uut;
};

TEST(HTTPRequestFactoryTest, createShutdownTest) {
    m_request->setURI(SHUTDOWN_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<CTSNCommon::ShutdownHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, createRootTest) {
    m_request->setURI(ROOT_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<Gateway::RootHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, createDataTest) {
    m_request->setURI(DATA_RESULT_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::DataHTTPRequestHandler* handler =
        dynamic_cast<Gateway::DataHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);
    POINTERS_EQUAL(handler->m_nodes, m_nodes);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createUartTxTest) {
    m_request->setURI(UART_TX_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::UartTxHTTPRequestHandler* handler = dynamic_cast<Gateway::UartTxHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_uart, m_uart);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createXBeeTxTest) {
    m_request->setURI(XBEE_TX_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    CTSNCommon::XBeeTxHTTPRequestHandler* handler =
        dynamic_cast<CTSNCommon::XBeeTxHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_uart, m_uart);
    POINTERS_EQUAL(handler->m_nodes, m_nodes);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createNodeStatusUpdateTest) {
    m_request->setURI(NODE_STATUS_UPDATE_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::NodeStatusUpdateHTTPRequestHandler* handler =
        dynamic_cast<Gateway::NodeStatusUpdateHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_nodes, m_nodes);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createNodeCheckUpdateTest) {
    m_request->setURI(NODE_CHECK_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::NodeCheckHTTPRequestHandler* handler =
        dynamic_cast<Gateway::NodeCheckHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_nodes, m_nodes);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createTextMessageTest) {
    m_request->setURI(TEXT_MESSAGE_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::TextMessageHTTPRequestHandler* handler = dynamic_cast<Gateway::TextMessageHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createEmailTest) {
    m_request->setURI(EMAIL_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::EmailHTTPRequestHandler* handler = dynamic_cast<Gateway::EmailHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createLogMessageTest) {
    m_request->setURI(LOG_MESSAGE_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::LogMessageHTTPRequestHandler* handler = dynamic_cast<Gateway::LogMessageHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createErrorMessageTest) {
    m_request->setURI(ERROR_MESSAGE_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::ErrorHTTPRequestHandler* handler = dynamic_cast<Gateway::ErrorHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createDatabasePokeTest) {
    m_request->setURI(DATABASE_POKE_URI);
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Gateway::DatabasePokeHTTPRequestHandler* handler = dynamic_cast<Gateway::DatabasePokeHTTPRequestHandler*>(m_uut->createRequestHandler(*m_request));

    CHECK(handler != nullptr);
    POINTERS_EQUAL(handler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(handler->m_mariadb, m_mariadb);

    delete handler;
}

TEST(HTTPRequestFactoryTest, notFoundTest) {
    m_request->setURI("herpaderp");
    m_request->set("user-agent", m_settings->getSetting("GATEWAY_AGENT"));

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<CTSNCommon::NotFoundHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, badClientTestBadUserAgent) {
    m_request->setURI(SHUTDOWN_URI);
    m_request->set("user-agent", "Firefox");

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<CTSNCommon::BadClientHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, badClientTestNoUserAgent) {
    m_request->setURI(SHUTDOWN_URI);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<CTSNCommon::BadClientHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

