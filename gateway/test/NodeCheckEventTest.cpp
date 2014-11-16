#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <UnitTest.h>

#include "gateway/Node.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/NodeCheckEvent.h"
#include "io/StringLogger.h"
#include "MockEventExecutor.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"

TEST_GROUP(NodeCheckEventTest) {
    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_nodes = new testing::StrictMock<Gateway::MockNodeContainer>();
        m_result = new testing::StrictMock<Gateway::MockMariaDBResult>();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::NodeCheckEvent(m_eventExecutor,
                                            m_mariadb,
                                            m_nodes,
                                            *m_errLogger);

        delete m_uut->m_result;
        m_uut->m_result = m_result;

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_result, m_result);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        // m_result is deleted in the uut.
        delete m_errLogger;
        delete m_nodes;
        delete m_mariadb;
        delete m_eventExecutor;
    }

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    testing::StrictMock<Gateway::MockMariaDBResult> *m_result;
    testing::StrictMock<Gateway::MockNodeContainer> *m_nodes;

    Common::IO::StringLogger *m_errLogger;
    Gateway::NodeCheckEvent *m_uut;
};

TEST(NodeCheckEventTest, successTest) {
    testing::InSequence dummy;

    Gateway::Node node1 (1, 0x01, Gateway::Node::NodeStatus::OKAY);
    Gateway::Node node3 (3, 0x03, Gateway::Node::NodeStatus::UNKNOWN);
    Gateway::Node node4 (4, 0x04, Gateway::Node::NodeStatus::BATTERY_CRITICAL);
    Gateway::Node node5 (5, 0x05, Gateway::Node::NodeStatus::OFFLINE);

    // What is returned from the query 
    std::vector<std::string> nodes = {std::to_string(node1.getID()),
                                      std::to_string(node3.getID()),
                                      std::to_string(node4.getID()),
                                      std::to_string(node5.getID())};

    EXPECT_CALL(*m_nodes, refreshNodes());

    // Perform database queries
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeCheckEvent::OUT_OF_DATE_NODE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(nodes));
    EXPECT_CALL(*m_result, freeResult());

    // Convert numbers to nodes, and set their status.
    EXPECT_CALL(*m_nodes, convertStringToNode(nodes[0]))
        .WillOnce(testing::Return(node1));

    // Node 1 should not be set, something else sets that status.

    // set node 3 status
    EXPECT_CALL(*m_nodes, convertStringToNode(nodes[1]))
        .WillOnce(testing::Return(node3));
    EXPECT_CALL(*m_nodes, setNodeStatus(node3.getID(), 
                                        Gateway::Node::NodeStatus::UNKNOWN))
        .WillOnce(testing::Return(false));

    // set node 4 status
    EXPECT_CALL(*m_nodes, convertStringToNode(nodes[2]))
        .WillOnce(testing::Return(node4));
    EXPECT_CALL(*m_nodes, setNodeStatus(node4.getID(),
                                        Gateway::Node::NodeStatus::UNKNOWN))
        .WillOnce(testing::Return(true));

    // since node 4 returned true, it means the new status is new.
    // log the error message.
    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    //Node 5 will be offline.  No setting should be done.
    EXPECT_CALL(*m_nodes, convertStringToNode(nodes[3]))
        .WillOnce(testing::Return(node5));

    //Execute
    m_uut->execute();

    Gateway::ErrorEvent *errEvent = dynamic_cast<Gateway::ErrorEvent*>(event.get());
    CHECK(errEvent != nullptr);

    CHECK_EQUAL(errEvent->m_errorNumber, Gateway::ErrorNumber::NODE_HAS_UNKNOWN_STATUS);
    CHECK_EQUAL(errEvent->m_node.getID(), node4.getID());

    POINTERS_EQUAL(errEvent->m_mariadb, m_mariadb);
}

TEST(NodeCheckEventTest, databaseExceptionTest) {
    std::string error = "error";

    EXPECT_CALL(*m_nodes, refreshNodes())
        .WillOnce(testing::Throw(std::runtime_error(error)));

    EXPECT_CALL(*m_result, freeResult());

    //Execute
    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

TEST(NodeCheckEventTest, nodeConversionFailureTest) {
    std::string error = "error";

    // What is returned from the query 
    std::vector<std::string> nodes = {"derp"};

    EXPECT_CALL(*m_nodes, refreshNodes());

    // Perform database queries
    EXPECT_CALL(*m_mariadb, mysql_real_query(Gateway::NodeCheckEvent::OUT_OF_DATE_NODE_QUERY));
    EXPECT_CALL(*m_result, storeResult());
    EXPECT_CALL(*m_result, getValuesFromColumn("id"))
        .WillOnce(testing::Return(nodes));
    EXPECT_CALL(*m_result, freeResult());

    // Throw exception
    EXPECT_CALL(*m_nodes, convertStringToNode(nodes[0]))
        .WillOnce(testing::Throw(std::invalid_argument(error)));

    //Execute
    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}


