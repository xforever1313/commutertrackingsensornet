#include <stdexcept>

#include <UnitTest.h>

#include "gateway/ErrorEvent.h"
#include "gateway/Node.h"
#include "gateway/NodeStatusUpdateEvent.h"
#include "io/StringLogger.h"
#include "MockEventExecutor.h"
#include "MockMariaDB.h"
#include "MockNodeContainer.h"

TEST_GROUP(GetNodeStatusEventTest) {

    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_nodes = new testing::StrictMock<Gateway::MockNodeContainer>();
        m_mariadb = new testing::StrictMock<Gateway::MockMariaDB>();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::NodeStatusUpdateEvent(nodeStatus, nodeID, 
                                                   m_nodes, 
                                                   m_eventExecutor,
                                                   m_mariadb,
                                                   *m_errLogger);

        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_mariadb, m_mariadb);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_errLogger;
        delete m_mariadb;
        delete m_nodes;
        delete m_eventExecutor;
    }

    static const Gateway::Node::NodeStatus nodeStatus = 
                    Gateway::Node::NodeStatus::OKAY;
    static const unsigned int nodeID = 1;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockNodeContainer> *m_nodes;
    testing::StrictMock<Gateway::MockMariaDB> *m_mariadb;
    Common::IO::StringLogger *m_errLogger;
    Gateway::NodeStatusUpdateEvent *m_uut;
};

TEST(GetNodeStatusEventTest, SuccessTestNoChange) {

    EXPECT_CALL(*m_nodes, setNodeStatus(nodeID, nodeStatus))
        .WillOnce(testing::Return(false));

    m_uut->execute();

    // No error occurred.
    CHECK(m_errLogger->getString().empty());
}
/*
TEST(GetNodeStatusEventTest, SuccessTestOkayChange) {

    EXPECT_CALL(*m_nodes, setNodeStatus(nodeID, 
                                        Gateway::Node::NodeStatus::OKAY))
        .WillOnce(testing::Return(false));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->execute();

    Gateway::ErrorEvent *errEvent = 
        dynamic_cast<Gateway::ErrorEvent*>(event.get()); 

    CHECK(errEvent != nullptr);

    // No error occurred.
    CHECK(m_errLogger->getString().empty());
}
*/
TEST(GetNodeStatusEventTest, outOfRangeFailure) {
    std::string error = "error";

    EXPECT_CALL(*m_nodes, setNodeStatus(nodeID, nodeStatus))
        .WillOnce(testing::Throw(std::out_of_range(error)));

    m_uut->execute();

    // Error logged.
    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

TEST(GetNodeStatusEventTest, invalidArgFailure) {
    std::string error = "error";

    EXPECT_CALL(*m_nodes, setNodeStatus(nodeID, nodeStatus))
        .WillOnce(testing::Throw(std::invalid_argument(error)));

    m_uut->execute();

    // Error logged.
    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

TEST(GetNodeStatusEventTest, runtimeErrorFailure) {
    std::string error = "error";

    EXPECT_CALL(*m_nodes, setNodeStatus(nodeID, nodeStatus))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    // Error logged.
    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}

