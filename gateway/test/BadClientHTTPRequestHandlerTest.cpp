#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "gateway/BadClientHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(BadClientHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_uut = new Gateway::BadClientHTTPRequestHandler();
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    Gateway::BadClientHTTPRequestHandler *m_uut;
};

TEST(BadClientHTTPRequestHandlerTest, handleRequestTest) {
    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::BadClientHTTPRequestHandler::MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
}
