#include <string>

#include "UnitTest.h"

#include "gateway/RootHTTPRequestHandler.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(RootHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_uut = new Gateway::RootHTTPRequestHandler();
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    Gateway::RootHTTPRequestHandler *m_uut;
};

TEST(RootHTTPRequestHandlerTest, postTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::RootHTTPRequestHandler::RESPONSE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(RootHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::RootHTTPRequestHandler::RESPONSE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

