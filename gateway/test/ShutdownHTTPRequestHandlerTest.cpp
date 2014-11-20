#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <sstream>

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
        m_shutdown = new testing::StrictMock<CTSNCommon::MockShutdown>();
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
    testing::StrictMock<CTSNCommon::MockShutdown> *m_shutdown;
    Gateway::ShutdownHTTPRequestHandler *m_uut;
};

TEST(ShutdownHTTPRequestHandlerTest, handlePostRequestShutdownTrueTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << Gateway::ShutdownHTTPRequestHandler::SHUTDOWN_FORM_DATA << "=true";

    EXPECT_CALL(*m_shutdown, shutdown());

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ShutdownHTTPRequestHandler::POST_TRUE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(ShutdownHTTPRequestHandlerTest, handlePostRequestShutdownFalseTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << Gateway::ShutdownHTTPRequestHandler::SHUTDOWN_FORM_DATA << "=false";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ShutdownHTTPRequestHandler::POST_FALSE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(ShutdownHTTPRequestHandlerTest, handlePostRequestShutdownMissingTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(ShutdownHTTPRequestHandlerTest, handleGetRequestTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::ShutdownHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
