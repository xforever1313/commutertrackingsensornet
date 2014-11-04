#include <memory>
#include <string>

#include "UnitTest.h"

#include "gateway/DataEvent.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockMariaDB.h"

TEST_GROUP(DataHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>;
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>;

        m_uut = new Gateway::DataHTTPRequestHandler(m_eventExecutor, m_mariadb);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mariadb;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;

    Gateway::DataHTTPRequestHandler *m_uut;
};

TEST(DataHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DataHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

