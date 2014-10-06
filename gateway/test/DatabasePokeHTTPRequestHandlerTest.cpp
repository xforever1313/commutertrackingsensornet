#include <string>

#include "UnitTest.h"

#include "gateway/DatabasePokeEvent.h"
#include "gateway/DatabasePokeHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "MockMariaDB.h"

TEST_GROUP(DatabasePokeHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_uut = new Gateway::DatabasePokeHTTPRequestHandler(m_mariadb, m_eventExecutor);

        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
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
    Gateway::DatabasePokeHTTPRequestHandler *m_uut;
};

TEST(DatabasePokeHTTPRequestHandlerTest, postRequestTrueTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    // Form data from a post request
    m_request->m_ss << Gateway::DatabasePokeHTTPRequestHandler::POKE_FORM_DATA << "=true";

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    //Ensure thing saved is of the right type
    Gateway::DatabasePokeEvent *databasePokeEvent = dynamic_cast<Gateway::DatabasePokeEvent*>(event.get());
    CHECK(databasePokeEvent != nullptr);

    //Ensure right things were sent to event
    POINTERS_EQUAL(databasePokeEvent->m_mariadb, m_mariadb);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DatabasePokeHTTPRequestHandler::POST_TRUE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(DatabasePokeHTTPRequestHandlerTest, postRequestFalseTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    // Form data from a post request
    m_request->m_ss << Gateway::DatabasePokeHTTPRequestHandler::POKE_FORM_DATA << "=false";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DatabasePokeHTTPRequestHandler::POST_FALSE_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);


}

TEST(DatabasePokeHTTPRequestHandlerTest, badRequestTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(DatabasePokeHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::DatabasePokeHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

