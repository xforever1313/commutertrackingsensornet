#include <map>
#include <string>

#include "UnitTest.h"

#include "ctsn_common/Node.h"
#include "ctsn_common/XBeeConstants.h"
#include "ctsn_common/XBeeHTTPTxEvent.h"
#include "CTSNSharedGlobals.py"
#include "io/StringLogger.h"
#include "MockEventExecutor.h"
#include "MockGPIOController.h"
#include "MockNodeContainer.h"
#include "MockUart.h"
#include "pi_node/BatteryCheckEvent.h"
#include "pi_node/PinNumbers.h"

TEST_GROUP(BatteryCheckEventTest) {

    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_gpio = new testing::StrictMock<CTSNCommon::MockGPIOController>();
        m_nodes = new testing::StrictMock<CTSNCommon::MockNodeContainer>();
        m_errLogger = new Common::IO::StringLogger();
        m_uart = new testing::StrictMock<CTSNCommon::MockUart>();
        m_uut = new PiNode::BatteryCheckEvent(m_nodes, m_eventExecutor, *m_gpio, m_uart, *m_errLogger);

        POINTERS_EQUAL(m_uut->m_nodes, m_nodes);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(&m_uut->m_gpio, m_gpio);
        POINTERS_EQUAL(m_uut->m_uart, m_uart);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_errLogger;
        delete m_uart;
        delete m_nodes;
        delete m_gpio;
        delete m_eventExecutor;
    }

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockGPIOController> *m_gpio;
    testing::StrictMock<CTSNCommon::MockNodeContainer> *m_nodes;
    Common::IO::StringLogger *m_errLogger;
    testing::StrictMock<CTSNCommon::MockUart> *m_uart;
    PiNode::BatteryCheckEvent *m_uut;
};

TEST(BatteryCheckEventTest, setTest) {

    EXPECT_CALL(*m_gpio, get(PiNode::PinNumbers::BATTERY_OKAY))
        .WillOnce(testing::Return(1));

    CTSNCommon::Node node (2, 0x02);
    EXPECT_CALL(*m_nodes, getCurrentNode())
        .WillOnce(testing::Return(node));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->execute();


    CTSNCommon::XBeeHTTPTxEvent *txEvent =
        dynamic_cast<CTSNCommon::XBeeHTTPTxEvent*>(event.get());
    CHECK(txEvent != nullptr);

    CHECK_EQUAL(txEvent->m_node.getID(), node.getID());
    POINTERS_EQUAL(txEvent->m_uart, m_uart);

    CHECK_EQUAL(txEvent->m_message, NODE_STATUS_UPDATE_URI + CTSNCommon::XBeeConstants::DATA_SEPARATOR +
                "node=" + std::to_string(node.getID()) + "|status=" + std::to_string(CTSNCommon::Node::OKAY));

    CHECK(m_errLogger->getString().empty());
}

TEST(BatteryCheckEventTest, clearTest) {

    EXPECT_CALL(*m_gpio, get(PiNode::PinNumbers::BATTERY_OKAY))
        .WillOnce(testing::Return(0));

    CTSNCommon::Node node (2, 0x02);
    EXPECT_CALL(*m_nodes, getCurrentNode())
        .WillOnce(testing::Return(node));

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->execute();


    CTSNCommon::XBeeHTTPTxEvent *txEvent =
        dynamic_cast<CTSNCommon::XBeeHTTPTxEvent*>(event.get());
    CHECK(txEvent != nullptr);

    CHECK_EQUAL(txEvent->m_node.getID(), node.getID());
    POINTERS_EQUAL(txEvent->m_uart, m_uart);

    CHECK_EQUAL(txEvent->m_message, NODE_STATUS_UPDATE_URI + CTSNCommon::XBeeConstants::DATA_SEPARATOR +
                "node=" + std::to_string(node.getID()) + "|status=" + std::to_string(CTSNCommon::Node::BATTERY_CRITICAL));

    CHECK(m_errLogger->getString().empty());

}

TEST(BatteryCheckEventTest, errorTest) {
    std::string error = "Error";

    EXPECT_CALL(*m_gpio, get(PiNode::PinNumbers::BATTERY_OKAY))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != std::string::npos);
}
