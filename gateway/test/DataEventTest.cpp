#include <stdexcept>
#include <string>

#include "UnitTest.h"

#include "gateway/DataEvent.h"
#include "ctsn_common/DataResultTypes.h"
#include "ctsn_common/Node.h"
#include "io/StringLogger.h"
#include "MockMariaDB.h"

TEST_GROUP(DataEventTest) {

    TEST_SETUP() {
        m_node = new CTSNCommon::Node(3, 0x03);
        m_type = CTSNCommon::DataResultType::WALKER;
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::DataEvent(*m_node, m_type,
                                       m_mariadb, *m_errLogger);

        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);

        m_expectedQuery = "INSERT INTO trail_result VALUES(NULL, NULL, 2, 3)";
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_errLogger;
        delete m_mariadb;
        delete m_node;
    }

    CTSNCommon::Node *m_node;
    CTSNCommon::DataResultType m_type;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    Common::IO::StringLogger *m_errLogger;
    std::string m_expectedQuery;
    Gateway::DataEvent *m_uut;
};

TEST(DataEventTest, goodQuery) {
    EXPECT_CALL(*m_mariadb, mysql_real_query(m_expectedQuery));
    EXPECT_CALL(*m_mariadb, mysql_commit());

    m_uut->execute();

    CHECK(m_errLogger->getString().empty());
  
}

TEST(DataEventTest, badQuery) {
    std::string error = "error";

    EXPECT_CALL(*m_mariadb, mysql_real_query(m_expectedQuery))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

TEST(DataEventTest, badCommit) {
    std::string error = "error";

    EXPECT_CALL(*m_mariadb, mysql_real_query(m_expectedQuery));

    EXPECT_CALL(*m_mariadb, mysql_commit())
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

