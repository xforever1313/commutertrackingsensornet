#include "UnitTest.h"
#include <cstdint>
#include <map>
#include <string>
#include <stdexcept>

#include "MockMariaDB.h"
#include "ctsn_common/Node.h"
#include "gateway/NodeContainer.h"

TEST_GROUP(NodeContainerTest) {
    TEST_SETUP() {
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_result = new testing::StrictMock<Gateway::MockMariaDBResult>();
        m_uut = new Gateway::NodeContainer(m_mariadb);
        delete m_uut->m_result;
        m_uut->m_result = m_result;

        //Ensure clearing the nodes leaves 1 node left, the broadcast node.
        //This node has an ID of 0 and an address of 0x000000000000FFFF
        CHECK_EQUAL(m_uut->m_nodes.size(), 1);
        CHECK_EQUAL(m_uut->m_nodes.at(0).getID(), 0);
        CHECK_EQUAL(m_uut->m_nodes.at(0).getAddress(),
                    Gateway::NodeContainer::BROADCAST_ADDRESS);
        CTSNCommon::Node node(1, 0x01);
        m_uut->m_nodes.insert(std::pair<unsigned int, CTSNCommon::Node> (1, node));
    }

    TEST_TEARDOWN() {
        // m_result is deleted in uut
        delete m_mariadb;
        delete m_uut;
    }

    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    testing::StrictMock<Gateway::MockMariaDBResult> *m_result;
    Gateway::NodeContainer *m_uut;
};

TEST(NodeContainerTest, refreshNodesSuccess) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {"1", "2", "3"};
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    m_uut->refreshNodes();

    CHECK_EQUAL(m_uut->m_nodes.size(), ids.size() + 1); // 1 for broadcast node
    CHECK_EQUAL(m_uut->m_nodes.at(0).getID(), 0);
    CHECK_EQUAL(m_uut->m_nodes.at(0).getAddress(), Gateway::NodeContainer::BROADCAST_ADDRESS);
    CHECK_EQUAL(m_uut->m_nodes.at(0).getStatus(), CTSNCommon::Node::NodeStatus::OKAY);
    for (size_t i = 1; i < m_uut->m_nodes.size(); ++i) {
        CHECK_EQUAL(m_uut->m_nodes.at(i).getID(), i);
        CHECK_EQUAL(m_uut->m_nodes.at(i).getAddress(), i);
        CHECK_EQUAL(m_uut->m_nodes.at(i).getStatus(), i);
    }
}

TEST(NodeContainerTest, refreshNodesMismatchedCols) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2"};
    std::vector<std::string> statuses = {"1", "2", "3"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::MISMATCHED_COLUMNS_MESSAGE);
    }
}

TEST(NodeContainerTest, refreshNodesMismatchedCols2) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {"1", "2"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::MISMATCHED_COLUMNS_MESSAGE);
    }
}

TEST(NodeContainerTest, refreshNodesBadID1) {
    std::vector<std::string> ids = {"1", "2", "asdf3"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {"1", "2", "3"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_DATABASE_DATA + " id: " + ids[2] + " address: " + addresses[2]);
    }
}

TEST(NodeContainerTest, refreshNodesBadID2) {
    std::vector<std::string> ids = {"1", "2", "3sdadgf"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {"1", "2", "3"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_DATABASE_DATA + " id: " + ids[2] + " address: " + addresses[2]);
    }
}

TEST(NodeContainerTest, refreshNodesBadAddress1) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "mndi3"};
    std::vector<std::string> statuses = {"1", "2", "3"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_DATABASE_DATA + " id: " + ids[2] + " address: " + addresses[2]);
    }
}


TEST(NodeContainerTest, refreshNodesBadAddress2) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "3asdfasdfsadf"};
    std::vector<std::string> statuses = {"1", "2", "3"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_DATABASE_DATA + " id: " + ids[2] + " address: " + addresses[2]);
    }
}

TEST(NodeContainerTest, refreshNodesBadStatus1) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {"1", "2", "asd3"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_DATABASE_DATA + " Bad Status: " + statuses[2]);
    }
}

TEST(NodeContainerTest, refreshNodesBadStatus2) {
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {"1", "2", "3asdfasdf"};

    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());

    try {
        m_uut->refreshNodes();
        FAIL("Exepcted exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_DATABASE_DATA + " Bad Status: " + statuses[2]);
    }
}

TEST(NodeContainerTest, refreshNodesMariaDBError) {
    std::string error = "error";
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    try {
        m_uut->refreshNodes();
        FAIL("Expected an exception.");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), error);
    }
}

TEST(NodeContainerTest, convertStringToNodeSuccess) {
    const CTSNCommon::Node node = m_uut->convertStringToNode("1");
    CHECK_EQUAL(node.getID(), 1);
}

TEST(NodeContainerTest, convertStringToNodeNoNumbersTest) {
    std::string s = "abc";
    try {
        m_uut->convertStringToNode(s);
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_NODE_MESSAGE + s);
    }
}

TEST(NodeContainerTest, convertStringToNodeNumberInFrontTest) {
    std::string s = "1abc";
    try {
        m_uut->convertStringToNode(s);
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_NODE_MESSAGE + s);
    }
}

TEST(NodeContainerTest, convertStringToNodeZeroTest) {
    const CTSNCommon::Node node = m_uut->convertStringToNode("0");
    CHECK_EQUAL(node.getID(), 0);
    CHECK_EQUAL(node.getAddress(), Gateway::NodeContainer::BROADCAST_ADDRESS);
}

TEST(NodeContainerTest, convertStringToNodeNumberTooHighTest) {
    std::string s = "10000000";
    try {
        m_uut->convertStringToNode(s);
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_NODE_MESSAGE + s);
    }
}

TEST(NodeContainerTest, setNodeStatusTestSuccess) {
    const unsigned int nodeID = 1; // From test setup

    // Needed for when refresh is called
    std::vector<std::string> ids = {"1", "2", "3"};
    std::vector<std::string> addresses = {"1", "2", "3"};
    std::vector<std::string> statuses = {std::to_string(CTSNCommon::Node::NodeStatus::OKAY), "2", "3"};

    std::string expectedQuery = Gateway::NodeContainer::SET_NODE_STATUS_QUERY +
                                std::to_string(CTSNCommon::Node::NodeStatus::OKAY) +
                                " WHERE id=" + std::to_string(nodeID);

    EXPECT_CALL(*m_mariadb, mysql_real_query(expectedQuery));
    EXPECT_CALL(*m_mariadb, mysql_commit());

    //Neded for when refresh is called.
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeContainer::DATABASE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(ids));
    EXPECT_CALL(*m_result, getValuesFromColumn("address"))
        .WillOnce(testing::Return(addresses));
    EXPECT_CALL(*m_result, getValuesFromColumn("status"))
        .WillOnce(testing::Return(statuses));
    EXPECT_CALL(*m_result, freeResult());


    bool changed = m_uut->setNodeStatus(nodeID, 
                                        CTSNCommon::Node::NodeStatus::OKAY);

    // okay is different than the default unknown, changed should be true.
    CHECK(changed);

    CTSNCommon::Node changedNode = m_uut->m_nodes.at(nodeID);
    CHECK_EQUAL(changedNode.getStatus(),
                CTSNCommon::Node::NodeStatus::OKAY);
}

TEST(NodeContainerTest, setNodeStatusTestNoChange) {
    const unsigned int nodeID = 1; // From test setup

    bool changed = m_uut->setNodeStatus(nodeID, 
                                        CTSNCommon::Node::NodeStatus::UNKNOWN);

    // Unknown is the same as the default status.  No change should happen.
    CHECK(!changed);

    CTSNCommon::Node changedNode = m_uut->m_nodes.at(nodeID);
    CHECK_EQUAL(changedNode.getStatus(),
                CTSNCommon::Node::NodeStatus::UNKNOWN);
}

