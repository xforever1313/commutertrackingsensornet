#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#include <map>
#include <string>

#define private public
#define protected public

#include "gateway/EmailEvent.h"
#include "io/StringLogger.h"
#include "MockEmailer.h"

TEST_GROUP(EmailEventTest) {
    TEST_SETUP() {
        m_subject = "subject";
        m_message = "message";
        m_addresses = { {"derp@derp.com", "derp"}, {"herp@herp.com", "herp"} };
    
        m_outLogger = new Common::IO::StringLogger();
        m_errLogger = new Common::IO::StringLogger();
        m_mockEmailer = new testing::StrictMock<Gateway::MockEmailer>();
        m_uut = new Gateway::EmailEvent(m_addresses, m_message, m_subject, *m_outLogger, *m_errLogger);
        
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
    std::map <std::string, std::string> m_addresses;
    
    Common::IO::StringLogger *m_outLogger;
    Common::IO::StringLogger *m_errLogger;
    testing::StrictMock<Gateway::MockEmailer> *m_mockEmailer;
    Gateway::EmailEvent *m_uut;
};

TEST(EmailEventTest, SendSuccess) {
    std::string expectedReturn = "Success!";
    EXPECT_CALL(*m_mockEmailer, send())
        .WillOnce(testing::Return(expectedReturn));
        
    m_uut->execute();
    
    //Ensure output log is what was expected
    CHECK_EQUAL(m_outLogger->getString(), expectedReturn + '\n');
    
    //Ensure error log is empty
    CHECK(m_errLogger->getString().empty());
}

TEST(EmailEventTest, SendFailException) {
    std::string expectedReturn = "Failure!";
    EXPECT_CALL(*m_mockEmailer, send())
        .WillOnce(testing::Throw(std::runtime_error(expectedReturn)));
        
    m_uut->execute();
    
    //Ensure output log is empty
    CHECK(m_outLogger->getString().empty());
    
    //Ensure error log is what was expected
    CHECK(m_errLogger->getString().find(expectedReturn) != std::string::npos);
}

