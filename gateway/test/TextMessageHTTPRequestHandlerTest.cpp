#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <memory>

#define private public
#define protected public

#include "EventInterface.h"
#include "gateway/Emailer.h"
#include "gateway/TextMessageEvent.h"
#include "gateway/TextMessageHTTPRequestHandler.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(TextMessageHTTPRequestHandlerTest) {

    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;

        m_eventExecutor = new testing::StrictMock<MockEventExecutor>;
        m_uut = new Gateway::TextMessageHTTPRequestHandler(m_eventExecutor);

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
    Gateway::TextMessageHTTPRequestHandler *m_uut;
};

TEST(TextMessageHTTPRequestHandlerTest, postMissingAllFieldsTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postMissingMessageFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "1,2&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postMissingSubjectFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "1,2&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postMissingNumbersFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "1,2&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postMissingProvidersFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postMismatchedNumbersAndProvidersFieldTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912,0123456789&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "1,2&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_NON_MATCHING_NUMBERS_AND_PROVIDERS_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postInvalidProviderZeroTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912,0123456789&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "0,2,3&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_INVALID_PROVIDER_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postInvalidProviderTenTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912,0123456789&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "10,2,3&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_INVALID_PROVIDER_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postInvalidProviderLetterTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << "1234567890,2345678912,0123456789&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "A,2,3&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << "some message&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << "some subject";


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_FAILURE_INVALID_PROVIDER_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(TextMessageHTTPRequestHandlerTest, postSuccessTest) {
    const std::string message = "hello world!";
    const std::string subject = "subject";
    const std::string number1 = "1234567890";
    const std::string number2 = "2345678901";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA << "=" << number1 << "," << number2 << "&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA << "=" << "1,2&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA << "=" << message << "&";
    m_request->m_ss << Gateway::TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA << "=" << subject << "&";

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::TextMessageEvent *textEvent = dynamic_cast<Gateway::TextMessageEvent*>(event.get());
    Gateway::Emailer *emailer = dynamic_cast<Gateway::Emailer*>(textEvent->m_emailer);
    CHECK_EQUAL(emailer->m_message, message);
    CHECK_EQUAL(emailer->m_subject, subject);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::POST_SUCCESS_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(TextMessageHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), Gateway::TextMessageHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
