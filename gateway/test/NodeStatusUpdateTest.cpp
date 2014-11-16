#include <stdexcept>

#include <UnitTest.h>

#include "gateway/Node.h"
#include "gateway/NodeStatusUpdateEvent.h"
#include "io/StringLogger.h"
#include "MockEventExecutor.h"
#include "MockNodeContainer.h"

TEST_GROUP(GetNodeStatusEventTest) {

    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_nodes = new testing::StrictMock<Gateway::MockNodeContainer>();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::NodeStatusUpdateEvent(nodeStatus, nodeID, 
                                                   m_nodes, 
                                                   m_eventExecutor,
                                                   *m_errLogger);

        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_errLogger;
        delete m_nodes;
        delete m_eventExecutor;
    }

    static const Gateway::Node::NodeStatus nodeStatus = 
                    Gateway::Node::NodeStatus::OKAY;
    static const unsigned int nodeID = 1;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<Gateway::MockNodeContainer> *m_nodes;
    Common::IO::StringLogger *m_errLogger;
    Gateway::NodeStatusUpdateEvent *m_uut;
};

TEST(GetNodeStatusEventTest, SuccessTest) {

    EXPECT_CALL(*m_nodes, setNodeStatus(nodeID, nodeStatus));

    m_uut->execute();

    // No error occurred.
    CHECK(m_errLogger->getString().empty());
}

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

