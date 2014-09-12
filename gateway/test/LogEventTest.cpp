#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "gateway/ErrorNumbers.h"
#include "gateway/LogEvent.h"
#include "io/StringLogger.h"
#include "MockMariaDB.h"

TEST_GROUP(LogEventTest) {
    TEST_SETUP() {
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_errorLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::LogEvent(Gateway::ErrorNumber::TEST_ERROR, m_node, m_mariadb, *m_errorLogger);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_errorLogger;
        delete m_mariadb;
    }

    const unsigned int m_node = 1;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    Common::IO::StringLogger *m_errorLogger;
    Gateway::LogEvent *m_uut;
};

TEST(LogEventTest, successTest){
    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_));
    EXPECT_CALL(*m_mariadb, mysql_commit());
    m_uut->execute();
}

TEST(LogEventTest, queryFail) {
    const std::string error("ERROR");
    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_))
        .WillOnce(testing::Throw(std::runtime_error(error)));

   m_uut->execute();

   CHECK_EQUAL(m_errorLogger->getString(), error + "\n"); 
}

TEST(LogEventTest, commitFail) {
    const std::string error("ERROR");
    EXPECT_CALL(*m_mariadb, mysql_real_query(testing::_));
    EXPECT_CALL(*m_mariadb, mysql_commit())
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK_EQUAL(m_errorLogger->getString(), error + "\n"); 
}

