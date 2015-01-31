#include "UnitTest.h"

#include "MockGPIOController.h"
#include "pi_node/StatusLed.h"

TEST_GROUP(StatusLEDTest) {
    TEST_SETUP() {
        m_pinNumber = 10;
        m_delay = 10;
        m_gpio = new testing::StrictMock<CTSNCommon::MockGPIOController> ();

        m_uut = new PiNode::StatusLed(m_pinNumber, m_delay, *m_gpio);

        CHECK(!m_uut->isShutdown());
        CHECK_EQUAL(m_uut->m_pinNumber, m_pinNumber);
        CHECK_EQUAL(m_uut->m_delay, m_delay);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_gpio;
    }

    unsigned int m_pinNumber;
    unsigned int m_delay = 10;

    testing::StrictMock<CTSNCommon::MockGPIOController> *m_gpio;
    PiNode::StatusLed *m_uut;
};

TEST(StatusLEDTest, runTest) {
    auto killFunc = [&](){m_uut->kill();};

    testing::InSequence dummy;

    EXPECT_CALL(*m_gpio, set(1, m_pinNumber));
    EXPECT_CALL(*m_gpio, set(0, m_pinNumber));
    EXPECT_CALL(*m_gpio, set(1, m_pinNumber));
    EXPECT_CALL(*m_gpio, set(0, m_pinNumber))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->run();

    CHECK(m_uut->isShutdown());
}
