#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "gateway/ShutdownHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockShutdown.h"

TEST_GROUP(ShutdownHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_shutdown = new testing::StrictMock<Gateway::MockShutdown>();
        m_uut = new Gateway::ShutdownHTTPRequestHandler(m_shutdown);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_shutdown;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    testing::StrictMock<Gateway::MockShutdown> *m_shutdown;
    Gateway::ShutdownHTTPRequestHandler *m_uut;
};

TEST(ShutdownHTTPRequestHandlerTest, handleRequestTest) {
    EXPECT_CALL(*m_shutdown, shutdown());
    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ShutdownHTTPRequestHandler::MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
