#include <stdexcept>

#include "UnitTest.h"

#include "gateway/DatabasePokeEvent.h"
#include "io/StringLogger.h"
#include "MockMariaDB.h"

TEST_GROUP(DatabasePokeEventTest) {
    TEST_SETUP() {
        m_errLogger = new Common::IO::StringLogger();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_uut = new Gateway::DatabasePokeEvent(m_mariadb, *m_errLogger);

        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mariadb;
        delete m_errLogger;
    }

    Common::IO::StringLogger *m_errLogger;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    Gateway::DatabasePokeEvent *m_uut;
};

TEST(DatabasePokeEventTest, successTest) {
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::DatabasePokeEvent::QUERY));

    m_uut->execute();

    //Ensure no errors occur
    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(DatabasePokeEventTest, FailTest) {
    std::string error = "error";
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::DatabasePokeEvent::QUERY))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

