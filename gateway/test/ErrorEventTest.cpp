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
#include "io/StringLogger.h"
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
    EXPECT_CALL(*m_errorMessageResult, getValuesFromColumn("MESSAGES"))
        .WillOnce(testing::Return(ret));

    m_uut->queryErrorMessage();

    CHECK_EQUAL(m_uut->m_errorMessage, std::to_string(NODE) + ": " + ret[0]);
}

TEST(ErrorEventTest, queryErrorMessageTestBadErrorNumber) {
    // No error numbers returned means someone derpered somewhere.
    std::vector<std::string> ret = {};

    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_));
    EXPECT_CALL(*m_errorMessageResult, storeResult());
    EXPECT_CALL(*m_errorMessageResult, getValuesFromColumn("MESSAGES"))
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
        CHECK_EQUAL(e.what(), Gateway::ErrorEvent::MISMATCHED_COLUMNS);
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
        CHECK_EQUAL(e.what(), Gateway::ErrorEvent::MISMATCHED_COLUMNS);
    }
}
