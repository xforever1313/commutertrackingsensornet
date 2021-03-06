#include <gmock/gmock.h>
#undef FAIL
#include <CppUTest/TestHarness.h>

#include <map>
#include <string>

#define private public
#define protected public

#include "gateway/Emailer.h"
#include "gateway/TextMessageEvent.h"
#include "io/StringLogger.h"
#include "MockEmailer.h"

TEST_GROUP(TextMessageEventTest) {
    TEST_SETUP() {
        m_subject = "subject";
        m_message = "message";
        m_numbers = { { "5181234567", Gateway::TextMessageEvent::Provider::VERIZON }, 
                      { "1112223333", Gateway::TextMessageEvent::Provider::ATT } };

        m_outLogger = new Common::IO::StringLogger();
        m_errLogger = new Common::IO::StringLogger();
        m_mockEmailer = new testing::StrictMock<Gateway::MockEmailer>();
        m_uut = new Gateway::TextMessageEvent(m_numbers, m_message, m_subject, *m_outLogger, *m_errLogger);

        delete m_uut->m_emailer;
        m_uut->m_emailer = m_mockEmailer;
    }

    TEST_TEARDOWN() {
        delete m_uut;
        // Emailer is deleted in EmailEvent's destructor

        delete m_errLogger;
        delete m_outLogger;
    }

    // "Constants"
    std::string m_subject;
    std::string m_message;
    std::map <std::string, Gateway::TextMessageEvent::Provider> m_numbers;

    Common::IO::StringLogger *m_outLogger;
    Common::IO::StringLogger *m_errLogger;
    testing::StrictMock<Gateway::MockEmailer> *m_mockEmailer;
    Gateway::TextMessageEvent *m_uut;
};

TEST(TextMessageEventTest, SendSuccess) {
    std::string expectedReturn = "Success!";
    EXPECT_CALL(*m_mockEmailer, send())
        .WillOnce(testing::Return(expectedReturn));

    m_uut->execute();

    //Ensure output log is what was expected
    CHECK(m_outLogger->getString().find(expectedReturn + '\n') != std::string::npos);

    //Ensure error log is empty
    CHECK(m_errLogger->getString().empty());
}

TEST(TextMessageEventTest, SendFailException) {
    std::string expectedReturn = "Failure!";
    EXPECT_CALL(*m_mockEmailer, send())
        .WillOnce(testing::Throw(std::runtime_error(expectedReturn)));

    m_uut->execute();

    //Ensure output log is empty
    CHECK(m_outLogger->getString().empty());

    //Ensure error log is what was expected
    CHECK(m_errLogger->getString().find(expectedReturn + '\n') != std::string::npos);
}

///convertStringToProviderTests
TEST(TextMessageEventTest, convertStringToProviderSuccess) {
    for (unsigned int i = Gateway::TextMessageEvent::ATT; i < Gateway::TextMessageEvent::UNKNOWN; ++i) {
        CHECK_EQUAL(Gateway::TextMessageEvent::convertStringToProvider(std::to_string(i)), i);
    }
}

TEST(TextMessageEventTest, convertStringToProviderFailures) {
    try {
        Gateway::TextMessageEvent::convertStringToProvider("0");
        FAIL("Should not have converted");
    }
    catch (const std::invalid_argument &e) {

    }

    try {
        Gateway::TextMessageEvent::convertStringToProvider("10");
        FAIL("Should not have converted");
    }
    catch (const std::invalid_argument &e) {

    }

    try {
        Gateway::TextMessageEvent::convertStringToProvider("1abc");
        FAIL("Should not have converted");
    }
    catch (const std::invalid_argument &e) {

    }

    try {
        Gateway::TextMessageEvent::convertStringToProvider("abc");
        FAIL("Should not have converted");
    }
    catch (const std::invalid_argument &e) {

    }
}

TEST(TextMessageEventTest, convertPhoneStringToNumberTest) {
    CHECK_EQUAL(Gateway::TextMessageEvent::convertPhoneStringToNumber("5555551234"), "5555551234");
    CHECK_EQUAL(Gateway::TextMessageEvent::convertPhoneStringToNumber("(555)-555-5555"), "5555555555");
    CHECK_EQUAL(Gateway::TextMessageEvent::convertPhoneStringToNumber("( 555 ) - 123   - 4569"), "5551234569");

    std::string badNumber = "123d564565";
    try{
        Gateway::TextMessageEvent::convertPhoneStringToNumber(badNumber);
        FAIL("Exception not thrown");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::TextMessageEvent::BAD_NUMBER_ERROR + " " + badNumber);
    }
}

TEST(TextMessageEventTest, badNumberConstructorTest) {
    std::string badNumber = "5181a234567";
    std::string goodNumber = "1112223333";
    std::map<std::string, Gateway::TextMessageEvent::Provider> numbers = 
        { { badNumber, Gateway::TextMessageEvent::Provider::VERIZON }, 
          { goodNumber, Gateway::TextMessageEvent::Provider::ATT } };

    Gateway::TextMessageEvent uut (numbers, m_message, m_subject, *m_outLogger, *m_errLogger);

    //Ensure error log is what was expected
    std::string expectedReturn = Gateway::TextMessageEvent::BAD_NUMBER_ERROR + " " + badNumber + "\n";
    CHECK(m_errLogger->getString().find(expectedReturn) != std::string::npos);

    //Ensure theres only one number in the emailer map.
    Gateway::Emailer *emailer = dynamic_cast<Gateway::Emailer*>(uut.m_emailer);

    CHECK_EQUAL(emailer->m_addresses.size(), 1);
}

