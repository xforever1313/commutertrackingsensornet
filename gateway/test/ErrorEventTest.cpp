#include <gmock/gmock.h>
#undef FAIL
#include <CppUTest/TestHarness.h>
#include <stdexcept>
#include <string>
#include <vector>

#define private public
#define protected public

#include "gateway/Emailer.h"
#include "gateway/EmailEvent.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/LogEvent.h"
#include "gateway/TextMessageEvent.h"
#include "io/StringLogger.h"
#include "MockEvent.h"
#include "MockMariaDB.h"

TEST_GROUP(ErrorEventTest) {
    TEST_SETUP() {
        m_errorMessageResult = new testing::StrictMock<Gateway::MockMariaDBResult>();
        m_userResult = new testing::StrictMock<Gateway::MockMariaDBResult>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_outLogger = new Common::IO::StringLogger();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::ErrorEvent(Gateway::ErrorNumber::TEST_ERROR,
                                        NODE, m_mariadb,
                                        *m_outLogger, *m_errLogger);

        delete m_uut->m_errorMessageResult;
        m_uut->m_errorMessageResult = m_errorMessageResult;

        delete m_uut->m_userResult;
        m_uut->m_userResult = m_userResult;

        CHECK_EQUAL(m_uut->m_node, NODE);
        CHECK_EQUAL(m_uut->m_errorNumber, Gateway::ErrorNumber::TEST_ERROR);
        POINTERS_EQUAL(m_uut->m_errorMessageResult, m_errorMessageResult);
        POINTERS_EQUAL(m_uut->m_userResult, m_userResult);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_errLogger;
        delete m_outLogger;
        delete m_mariadb;

        //The results are deleted in m_uut
    }

    const static unsigned int NODE = 2;
    testing::StrictMock<Gateway::MockMariaDBResult> *m_errorMessageResult;
    testing::StrictMock<Gateway::MockMariaDBResult> *m_userResult;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    Common::IO::StringLogger *m_outLogger;
    Common::IO::StringLogger *m_errLogger;
    Gateway::ErrorEvent *m_uut;
};

///queryErrorMessage() tests
TEST(ErrorEventTest, queryErrorMessageTestSuccess) {
    std::vector<std::string> ret = {"Test Errorr!"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_));
    EXPECT_CALL(*m_errorMessageResult, storeResult());
    EXPECT_CALL(*m_errorMessageResult, getValuesFromColumn("MESSAGE"))
        .WillOnce(testing::Return(ret));

    m_uut->queryErrorMessage();

    CHECK_EQUAL(m_uut->m_errorMessage, std::to_string(NODE) + ": " + ret[0]);
}

TEST(ErrorEventTest, queryErrorMessageTestBadErrorNumber) {
    // No error numbers returned means someone derpered somewhere.
    std::vector<std::string> ret = {};

    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_));
    EXPECT_CALL(*m_errorMessageResult, storeResult());
    EXPECT_CALL(*m_errorMessageResult, getValuesFromColumn("MESSAGE"))
        .WillOnce(testing::Return(ret));

    try {
        m_uut->queryErrorMessage();
        FAIL("Expected an exception");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::ErrorEvent::INVALID_ERROR_NUMBER);
    }
}

TEST(ErrorEventTest, queryErrorMessageTestBadQuery) {
    std::string error = "error";

    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    try {
        m_uut->queryErrorMessage();
        FAIL("Expected an exception");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), error);
    }
}

///queryForStaffUsers tests
TEST(ErrorEventTest, queryForStaffedUsersSuccessTest) {
    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_));
    EXPECT_CALL(*m_userResult, storeResult());

    m_uut->queryForStaffedUsers();
}

TEST(ErrorEventTest, queryForStaffedUsersBadQuery) {
    std::string error = "error";

    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    try {
        m_uut->queryForStaffedUsers();
        FAIL("Expected an exception");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), error);
    }
}

///setupEmailEvent Tests
TEST(ErrorEventTest, setupEmailEventSuccessTest) {
    std::vector<std::string> addresses = {"seth@derp.com", "alex@herp.com", "", "jared@berp.com"};
    std::vector<std::string> firstNames = {"Seth", "Alex", "blah", "Jared"};
    std::vector<std::string> lastNames = {"Blah", "Derp", "guy", "Herp"}; 
    m_uut->m_errorMessage = "Error Message";

    EXPECT_CALL(*m_userResult, getValuesFromColumn("email"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("first_name"))
        .WillOnce(testing::Return(firstNames));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("last_name"))
        .WillOnce(testing::Return(lastNames));
    
    m_uut->setupEmailEvent();

    // Ensure email event was created correctly.
    CHECK(m_uut->m_emailEvent != nullptr);
    Gateway::EmailEvent *emailEvent = dynamic_cast<Gateway::EmailEvent*>(m_uut->m_emailEvent);
    CHECK(emailEvent != nullptr);

    // Convert emailer
    Gateway::Emailer *emailer = dynamic_cast<Gateway::Emailer*>(emailEvent->m_emailer);
    CHECK(emailer != nullptr);

    // Confirm everything worked
    try {
        CHECK_EQUAL(emailer->m_subject, Gateway::ErrorEvent::SUBJECT);
        CHECK_EQUAL(emailer->m_message, m_uut->m_errorMessage);

        CHECK_EQUAL(emailer->m_addresses.at(addresses[0]), firstNames[0] + " " + lastNames[0]);
        CHECK_EQUAL(emailer->m_addresses.at(addresses[1]), firstNames[1] + " " + lastNames[1]);
        CHECK_EQUAL(emailer->m_addresses.at(addresses[3]), firstNames[3] + " " + lastNames[3]);

        // Ensure "" did not make it in
        try {
            emailer->m_addresses.at(addresses[2]);
            FAIL("Expected an out of range exception");
        }
        catch (const std::out_of_range &e) {
            CHECK(true);
        }
    }
    catch (const std::out_of_range &e) {
        FAIL("Test failed due to a key not making it in");
    }
}

TEST(ErrorEventTest, setupEmailEventFirstNameMismatchedSizeTest) {
    std::vector<std::string> addresses = {"seth@derp.com", "alex@herp.com", "", "jared@berp.com"};
    std::vector<std::string> firstNames = {"Seth", "Alex", "Jared"};
    std::vector<std::string> lastNames = {"Blah", "Derp", "guy", "Herp"}; 
    m_uut->m_errorMessage = "Error Message";

    EXPECT_CALL(*m_userResult, getValuesFromColumn("email"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("first_name"))
        .WillOnce(testing::Return(firstNames));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("last_name"))
        .WillOnce(testing::Return(lastNames));

    try {    
        m_uut->setupEmailEvent();
        FAIL("Expected Exception");
    }
    catch (const std::runtime_error &e) {
        CHECK(std::string(e.what()).find(Gateway::ErrorEvent::MISMATCHED_COLUMNS) != std::string::npos);
    }
}

TEST(ErrorEventTest, setupEmailEventLastNameMismatchedSizeTest) {
    std::vector<std::string> addresses = {"seth@derp.com", "alex@herp.com", "", "jared@berp.com"};
    std::vector<std::string> firstNames = {"Seth", "Alex", "blah", "Jared"};
    std::vector<std::string> lastNames = {"Blah", "Derp", "guy"}; 
    m_uut->m_errorMessage = "Error Message";

    EXPECT_CALL(*m_userResult, getValuesFromColumn("email"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("first_name"))
        .WillOnce(testing::Return(firstNames));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("last_name"))
        .WillOnce(testing::Return(lastNames));

    try {    
        m_uut->setupEmailEvent();
        FAIL("Expected Exception");
    }
    catch (const std::runtime_error &e) {
         CHECK(std::string(e.what()).find(Gateway::ErrorEvent::MISMATCHED_COLUMNS) != std::string::npos);
    }
}

///setupLogEvent Tests
TEST(ErrorEventTest, setupLogEventTest) {
    m_uut->setupLogEvent();

    CHECK(m_uut->m_logEvent != nullptr);
    Gateway::LogEvent *logEvent = dynamic_cast<Gateway::LogEvent*>(m_uut->m_logEvent);
    CHECK(logEvent != nullptr);

    CHECK_EQUAL(logEvent->m_errorNumber, m_uut->m_errorNumber);
    CHECK_EQUAL(logEvent->m_node, NODE);
    POINTERS_EQUAL(logEvent->m_mariadb, m_mariadb);
}

///setupTextEvents
TEST(ErrorEventTest, setupTextMessageEventSuccessTest) {
    std::vector<std::string> numbers = {"1234567890", "2345678901", "", "1591234568"};
    std::vector<std::string> providers = {"1", "2", "3", "4"};
    m_uut->m_errorMessage = "Error Message";

    EXPECT_CALL(*m_userResult, getValuesFromColumn("PHONE_NUMBER"))
        .WillOnce(testing::Return(numbers));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("PROVIDER"))
        .WillOnce(testing::Return(providers));
    
    m_uut->setupTextEvent();

    // Ensure email event was created correctly.
    CHECK(m_uut->m_textMessageEvent != nullptr);
    Gateway::TextMessageEvent *textMessageEvent = dynamic_cast<Gateway::TextMessageEvent*>(m_uut->m_textMessageEvent);
    CHECK(textMessageEvent != nullptr);

    // Convert emailer
    Gateway::Emailer *emailer = dynamic_cast<Gateway::Emailer*>(textMessageEvent->m_emailer);
    CHECK(emailer != nullptr);

    // Confirm everything worked
    try {
        CHECK_EQUAL(emailer->m_subject, Gateway::ErrorEvent::SUBJECT);
        CHECK_EQUAL(emailer->m_message, m_uut->m_errorMessage);

        CHECK_EQUAL(emailer->m_addresses.at(numbers[0] + "@" + Gateway::TextMessageEvent::PROVIDER_EMAIL.at(Gateway::TextMessageEvent::ATT)), numbers[0]);
        CHECK_EQUAL(emailer->m_addresses.at(numbers[1] + "@" + Gateway::TextMessageEvent::PROVIDER_EMAIL.at(Gateway::TextMessageEvent::VERIZON)), numbers[1]);
        CHECK_EQUAL(emailer->m_addresses.at(numbers[3] + "@" + Gateway::TextMessageEvent::PROVIDER_EMAIL.at(Gateway::TextMessageEvent::SPRINT)), numbers[3]);

        // Ensure "" did not make it in
        try {
            emailer->m_addresses.at(numbers[2]);
            FAIL("Expected an out of range exception");
        }
        catch (const std::out_of_range &e) {
            CHECK(true);
        }
    }
    catch (const std::out_of_range &e) {

        FAIL("Test failed due to a key not making it in");
    }
}


TEST(ErrorEventTest, setupTextEventLastNameMismatchedSizeTest) {
    std::vector<std::string> numbers = {"1234567890", "2345678901", "", "1591234568"};
    std::vector<std::string> providers = {"1", "2", "3", "4", "5"};

    EXPECT_CALL(*m_userResult, getValuesFromColumn("PHONE_NUMBER"))
        .WillOnce(testing::Return(numbers));
    EXPECT_CALL(*m_userResult, getValuesFromColumn("PROVIDER"))
        .WillOnce(testing::Return(providers));

    try {    
        m_uut->setupTextEvent();
        FAIL("Expected Exception");
    }
    catch (const std::runtime_error &e) {
        CHECK(std::string(e.what()).find(Gateway::ErrorEvent::MISMATCHED_COLUMNS) != std::string::npos);
    }
}

///ExecuteEvents Tests
TEST(ErrorEventTest, executeEventTestSuccess) {
    testing::StrictMock<Common::MockEvent> *emailEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *logEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *textEvent = new testing::StrictMock<Common::MockEvent>;
   
    m_uut->m_emailEvent = emailEvent;
    m_uut->m_logEvent = logEvent;
    m_uut->m_textMessageEvent = textEvent;

    EXPECT_CALL(*logEvent, execute());
    EXPECT_CALL(*textEvent, execute());
    EXPECT_CALL(*emailEvent, execute());
   
     m_uut->executeEvents();

    //Events are deleted in uut's destructor
}

TEST(ErrorEventTest, executeEventTestLogFailure) {
    std::string error = "error";

    testing::StrictMock<Common::MockEvent> *emailEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *logEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *textEvent = new testing::StrictMock<Common::MockEvent>;
   
    m_uut->m_emailEvent = emailEvent;
    m_uut->m_logEvent = logEvent;
    m_uut->m_textMessageEvent = textEvent;

    EXPECT_CALL(*logEvent, execute())
        .WillOnce(testing::Throw(std::invalid_argument(error)));
    EXPECT_CALL(*textEvent, execute());
    EXPECT_CALL(*emailEvent, execute());

    m_uut->executeEvents();
    CHECK_EQUAL(m_errLogger->getString(), error + '\n');
    
    //Events are deleted in uut's destructor
}

TEST(ErrorEventTest, executeEventTestTextFailure) {
    std::string error = "error";

    testing::StrictMock<Common::MockEvent> *emailEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *logEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *textEvent = new testing::StrictMock<Common::MockEvent>;
   
    m_uut->m_emailEvent = emailEvent;
    m_uut->m_logEvent = logEvent;
    m_uut->m_textMessageEvent = textEvent;

    EXPECT_CALL(*logEvent, execute());
    EXPECT_CALL(*textEvent, execute())
        .WillOnce(testing::Throw(std::invalid_argument(error)));
    EXPECT_CALL(*emailEvent, execute());

    m_uut->executeEvents();
    CHECK_EQUAL(m_errLogger->getString(), error + '\n');

    //Events are deleted in uut's destructor
}

TEST(ErrorEventTest, executeEventTestEmailFailure) {
    std::string error = "error";

    testing::StrictMock<Common::MockEvent> *emailEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *logEvent = new testing::StrictMock<Common::MockEvent>;
    testing::StrictMock<Common::MockEvent> *textEvent = new testing::StrictMock<Common::MockEvent>;
   
    m_uut->m_emailEvent = emailEvent;
    m_uut->m_logEvent = logEvent;
    m_uut->m_textMessageEvent = textEvent;

    EXPECT_CALL(*logEvent, execute());
    EXPECT_CALL(*textEvent, execute());
    EXPECT_CALL(*emailEvent, execute())
        .WillOnce(testing::Throw(std::invalid_argument(error)));

    m_uut->executeEvents();
    CHECK_EQUAL(m_errLogger->getString(), error + '\n');

    //Events are deleted in uut's destructor
}

///Execute test
TEST(ErrorEventTest, executeTest) {
    std::string error = "error";
    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK_EQUAL(m_errLogger->getString(), error + '\n');
}

