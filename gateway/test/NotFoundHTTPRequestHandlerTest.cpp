#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "gateway/NotFoundHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(NotFoundHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_uut = new Gateway::NotFoundHTTPRequestHandler();
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    Gateway::NotFoundHTTPRequestHandler *m_uut;
};

TEST(NotFoundHTTPRequestHandlerTest, handleRequestTest) {
    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::NotFoundHTTPRequestHandler::MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
}
