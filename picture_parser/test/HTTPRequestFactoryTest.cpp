#include <Poco/Net/HTTPRequestHandler.h>

#include "UnitTest.h"

#include "CTSNSharedGlobals.py"
#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "EventExecutorInterface.h"
#include "picture_parser/CVRunnerInterface.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/HTTPRequestFactory.h"
#include "picture_parser/PictureParseHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockEventExecutor.h"
#include "MockCVRunner.h"
#include "MockHTTPPoster.h"
#include "MockEventExecutor.h"
#include "MockShutdown.h"
#include "Secrets.py"

TEST_GROUP(HTTPRequestFactoryTest) {
    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_request = new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_shutdown = new testing::StrictMock<CTSNCommon::MockShutdown> ();
        m_cvRunner = new testing::StrictMock<PictureParser::MockCVRunner>();
        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();
        m_pc = new PictureParser::PictureContainer();
        m_uut = new PictureParser::HTTPRequestFactory(m_shutdown,
                                                      m_httpPoster,
                                                      m_pc,
                                                      m_cvRunner,
                                                      m_eventExecutor);

        POINTERS_EQUAL(m_uut->m_shutdown, m_shutdown);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_cvRunner, m_cvRunner);
        POINTERS_EQUAL(m_uut->m_pc, m_pc);
        POINTERS_EQUAL(m_uut->m_httpPoster, m_httpPoster);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_shutdown;
        delete m_request;
        delete m_eventExecutor;
        delete m_pc;
        delete m_httpPoster;
        delete m_cvRunner;
    }

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<CTSNCommon::MockShutdown> *m_shutdown;
    testing::StrictMock<PictureParser::MockCVRunner> *m_cvRunner;
    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;
    PictureParser::PictureContainer *m_pc;
    PictureParser::HTTPRequestFactory *m_uut;
};

TEST(HTTPRequestFactoryTest, createShutdownTest) {
    m_request->setURI(SHUTDOWN_URI);
    m_request->set("user-agent", USER_AGENT);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<CTSNCommon::ShutdownHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}

TEST(HTTPRequestFactoryTest, createDataTest) {
    m_request->setURI(DATA_URI);
    m_request->set("user-agent", USER_AGENT);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<PictureParser::PictureParseHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}


TEST(HTTPRequestFactoryTest, notFoundTest) {
    m_request->setURI("herpaderp");
    m_request->set("user-agent", USER_AGENT);

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

