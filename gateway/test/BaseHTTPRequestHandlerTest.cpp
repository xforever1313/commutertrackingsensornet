#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "gateway/BaseHTTPRequestHandler.h"
#include "MockHTTPServerResponse.h"

class BaseHTTPRequestHandlerImpl : public Gateway::BaseHTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override {
            //This is just here to implement the abstract class.
        }
};

TEST_GROUP(BaseHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;
        m_uut = new BaseHTTPRequestHandlerImpl;
        m_expectedMessage = "This is a server response!";

    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_response;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    Gateway::BaseHTTPRequestHandler *m_uut;
    std::string m_expectedMessage;
};

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
