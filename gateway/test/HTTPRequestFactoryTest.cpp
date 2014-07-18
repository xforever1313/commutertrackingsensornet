#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <Poco/Net/HTTPRequestHandler.h>

#define private public
#define protected public

#include "CTSNSharedGlobals.py"
#include "gateway/HTTPRequestFactory.h"
#include "gateway/ShutdownHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockShutdown.h"

TEST_GROUP(HTTPRequestFactoryTest) {
    TEST_SETUP() {
        m_request = new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_shutdown = new testing::StrictMock<Gateway::MockShutdown> ();
        m_uut = new Gateway::HTTPRequestFactory(m_shutdown);
    }

    TEST_TEARDOWN() {
        delete m_shutdown;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<Gateway::MockShutdown> *m_shutdown;
    Gateway::HTTPRequestFactory *m_uut;
};

TEST(HTTPRequestFactoryTest, createShutdownTest) {
    m_request->setURI(SHUTDOWN_URI);

    Poco::Net::HTTPRequestHandler *handler = m_uut->createRequestHandler(*m_request);
    CHECK(dynamic_cast<Gateway::ShutdownHTTPRequestHandler*>(handler) != nullptr);
    delete handler;
}
