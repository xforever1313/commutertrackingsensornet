#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <memory>

#define private public
#define protected public

#include "EventInterface.h"
#include "gateway/Emailer.h"
#include "gateway/EmailEvent.h"
#include "gateway/EmailHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(EmailHTTPRequestHandlerTest) {

    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>;
        m_uut = new Gateway::EmailHTTPRequestHandler(m_eventExecutor);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    Gateway::EmailHTTPRequestHandler *m_uut;
};

TEST(EmailHTTPRequestHandlerTest, postMissingAllFieldsTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(EmailHTTPRequestHandlerTest, postMissingMessageFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::EmailHTTPRequestHandler::ADDRESS_FORM_DATA << "=" << "seth@derp.com,herp@derp.com&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::NAMES_FORM_DATA << "=" << "seth,herp&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(EmailHTTPRequestHandlerTest, postMissingSubjectFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::EmailHTTPRequestHandler::ADDRESS_FORM_DATA << "=" << "seth@derp.com,herp@derp.com&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::NAMES_FORM_DATA << "=" << "seth,herp&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(EmailHTTPRequestHandlerTest, postMissingNamesFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::EmailHTTPRequestHandler::ADDRESS_FORM_DATA << "=" << "seth@derp.com,herp@derp.com&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(EmailHTTPRequestHandlerTest, postMissingAddressesFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::EmailHTTPRequestHandler::NAMES_FORM_DATA << "=" << "seth,herp&";    
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(EmailHTTPRequestHandlerTest, postMismatchedAddressesAndNames) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::EmailHTTPRequestHandler::ADDRESS_FORM_DATA << "=" << "seth@derp.com,herp@derp.com,blah@derp.com&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::NAMES_FORM_DATA << "=" << "seth,herp&";    
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_FAILURE_NON_MATCHING_ADDRESSES_AND_NAMES_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(EmailHTTPRequestHandlerTest, postSuccessTest) {
    const std::string message = "hello world!";
    const std::string subject = "subject";
    const std::string addr1 = "seth@derp.com";
    const std::string addr2 = "blah@derp.com";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::ADDRESS_FORM_DATA << "=" << addr1 << "," << addr2 << "&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::NAMES_FORM_DATA << "=" << "seth,blah&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << message << "&";
    m_request->m_ss << Gateway::EmailHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << subject << "&";

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::EmailEvent *emailEvent = dynamic_cast<Gateway::EmailEvent*>(event.get());
    Gateway::Emailer *emailer = dynamic_cast<Gateway::Emailer*>(emailEvent->m_emailer);
    CHECK_EQUAL(emailer->m_message, message);
    CHECK_EQUAL(emailer->m_subject, subject);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::POST_SUCCESS_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(EmailHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::EmailHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

