#include <Poco/Net/HTTPRequestHandler.h>

#include "UnitTest.h"

#include "CTSNSharedGlobals.py"
#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "EventExecutorInterface.h"
#include "pi_node/BatteryCheckHTTPRequestHandler.h"
#include "pi_node/HTTPRequestFactory.h"
#include "pi_node/PictureParseHTTPRequestHandler.h"
#include "MockGPIOController.h"
#include "MockHTTPServerRequest.h"
#include "MockEventExecutor.h"
#include "MockNodeContainer.h"
#include "MockShutdown.h"
#include "MockUart.h"

TEST_GROUP(HTTPRequestFactoryTest) {
    TEST_SETUP() {
        m_settings = &CTSNCommon::Settings::getInstance();
        m_settings->m_stringSettings["NODE_AGENT"] = "Node Agent";

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_cvExecutor = new testing::StrictMock<MockEventExecutor>();
        m_request = new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_gpio = new testing::StrictMock<CTSNCommon::MockGPIOController>();
        m_nodes = new testing::StrictMock<CTSNCommon::MockNodeContainer>();
        m_shutdown = new testing::StrictMock<CTSNCommon::MockShutdown> ();
        m_uart = new testing::StrictMock<CTSNCommon::MockUart>();
        m_uut = new PiNode::HTTPRequestFactory(m_shutdown,
                                               *m_gpio,
                                               m_nodes,
                                               m_eventExecutor,
                                               m_cvExecutor,
                                               m_uart);

        POINTERS_EQUAL(m_uut->m_shutdown, m_shutdown);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(&m_uut->m_gpio, m_gpio);
        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_uart;
        delete m_shutdown;
        delete m_nodes;
        delete m_gpio;
        delete m_request;
        delete m_cvExecutor;
        delete m_eventExecutor;
    }

    CTSNCommon::Settings *m_settings;
    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<MockEventExecutor> *m_cvExecutor;
    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<CTSNCommon::MockGPIOController> *m_gpio;
    testing::StrictMock<CTSNCommon::MockNodeContainer> *m_nodes;
    testing::StrictMock<CTSNCommon::MockShutdown> *m_shutdown;
    testing::StrictMock<CTSNCommon::MockUart> *m_uart;
    PiNode::HTTPRequestFactory *m_uut;
};

TEST(HTTPRequestFactoryTest, createShutdownTest) {
    m_request->setURI(SHUTDOWN_URI);
    m_request->set("user-agent", m_settings->getSetting("NODE_AGENT"));

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<CTSNCommon::ShutdownHTTPRequestHandler*>(handler) != nullptr);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createBatteryCheckTest) {
    m_request->setURI(BATTERY_CHECK_URI);
    m_request->set("user-agent", m_settings->getSetting("NODE_AGENT"));

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    PiNode::BatteryCheckHTTPRequestHandler *batHandler = dynamic_cast<PiNode::BatteryCheckHTTPRequestHandler*>(handler);
    CHECK(batHandler != nullptr);
    POINTERS_EQUAL(batHandler->m_eventExecutor, m_eventExecutor);
    POINTERS_EQUAL(&batHandler->m_gpio, m_gpio);
    POINTERS_EQUAL(batHandler->m_nodes, m_nodes);
    POINTERS_EQUAL(batHandler->m_uart, m_uart);

    delete handler;
}

TEST(HTTPRequestFactoryTest, createPictureParseTest) {
    m_request->setURI(PICTURE_PARSE_URI);
    m_request->set("user-agent", m_settings->getSetting("NODE_AGENT"));

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    PiNode::PictureParseHTTPRequestHandler *ppHandler = dynamic_cast<PiNode::PictureParseHTTPRequestHandler*>(handler);
    CHECK(ppHandler != nullptr);
    POINTERS_EQUAL(ppHandler->m_cvExecutor, m_cvExecutor);

    delete handler;
}

TEST(HTTPRequestFactoryTest, notFoundTest) {
    m_request->setURI("herpaderp");
    m_request->set("user-agent", m_settings->getSetting("NODE_AGENT"));

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

