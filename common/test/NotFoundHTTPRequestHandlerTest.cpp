#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(NotFoundHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_uut = new CTSNCommon::NotFoundHTTPRequestHandler();
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    CTSNCommon::NotFoundHTTPRequestHandler *m_uut;
};

TEST(NotFoundHTTPRequestHandlerTest, handlePostRequestTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::NotFoundHTTPRequestHandler::MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
}

TEST(NotFoundHTTPRequestHandlerTest, handleGetRequestTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), CTSNCommon::NotFoundHTTPRequestHandler::MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
}

