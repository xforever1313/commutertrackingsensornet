#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "MockHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(BaseHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request = new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;
        m_uut = new Gateway::MockHTTPRequestHandler;
        m_expectedMessage = "This is a server response!";

    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    Gateway::MockHTTPRequestHandler *m_uut; ///< Mock is used to override the abstract functions
    std::string m_expectedMessage;
};


TEST(BaseHTTPRequestHandlerTest, handleGetRequestCalledTest){
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK(m_uut->m_getCalled);
    CHECK(!m_uut->m_postCalled);
}


TEST(BaseHTTPRequestHandlerTest, handlePostRequestCalledTest){
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK(!m_uut->m_getCalled);
    CHECK(m_uut->m_postCalled);
}

TEST(BaseHTTPRequestHandlerTest, handleInvalidMethodTest){
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_PUT);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK(!m_uut->m_getCalled);
    CHECK(!m_uut->m_postCalled);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(BaseHTTPRequestHandlerTest, sendSuccessResponseTest) {
    m_uut->sendSuccessResponse(*m_response, m_expectedMessage);

    CHECK_EQUAL(m_response->m_response.str(), m_expectedMessage);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(BaseHTTPRequestHandlerTest, sendNotFoundResponseTest) {
    m_uut->sendNotFoundResponse(*m_response, m_expectedMessage);

    CHECK_EQUAL(m_response->m_response.str(), m_expectedMessage);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
}

TEST(BaseHTTPRequestHandlerTest, sendBadRequestResponseTest) {
    m_uut->sendBadRequestResponse(*m_response, m_expectedMessage);

    CHECK_EQUAL(m_response->m_response.str(), m_expectedMessage);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(BaseHTTPRequestHandlerTest, sendForbiddenResponseTest) {
    m_uut->sendForbiddenResponse(*m_response, m_expectedMessage);

    CHECK_EQUAL(m_response->m_response.str(), m_expectedMessage);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
}
