#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <stdexcept>

#define private public
#define protected public

#include "ctsn_common/UartRecvThread.h"
#include "io/StringLogger.h"
#include "MockUart.h"
#include "MockUartRecvCallback.h"

TEST_GROUP(UartRecvThreadTest) {

    TEST_SETUP() {
        m_data = {0x00, 0x01, 0x02, 0xFF};

        m_errorLogger = new Common::IO::StringLogger();
        m_mockUart = new testing::StrictMock<CTSNCommon::MockUart>();
        m_callback = new testing::StrictMock<CTSNCommon::MockUartRecvCallback>();
        m_uut = new CTSNCommon::UartRecvThread(m_mockUart, m_callback,
                                               *m_errorLogger);

        CHECK(m_uut->m_isAlive);
        CHECK(m_uut->isAlive());
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_callback;
        delete m_mockUart;
        delete m_errorLogger;
    }

    std::vector<std::uint8_t> m_data;

    Common::IO::StringLogger *m_errorLogger;
    testing::StrictMock<CTSNCommon::MockUart> *m_mockUart;
    testing::StrictMock<CTSNCommon::MockUartRecvCallback> *m_callback;
    CTSNCommon::UartRecvThread *m_uut;
};

TEST(UartRecvThreadTest, killTest) {
    m_uut->kill();

    CHECK(!m_uut->m_isAlive);
    CHECK(!m_uut->isAlive());
}

TEST(UartRecvThreadTest, runSuccessTest) {

    testing::InSequence dummy;

    EXPECT_CALL(*m_mockUart, recvBinary())
        .WillOnce(testing::Return(m_data));

    EXPECT_CALL(*m_callback, addData(m_data));

    m_uut->start();
    m_uut->dataReady();
    m_uut->kill();
    m_uut->join();

    // Ensure no errors
    CHECK_EQUAL(m_errorLogger->getString(), "");
}

TEST(UartRecvThreadTest, runFailTest) {
    std::string errorMessage = "Hello ";

    testing::InSequence dummy;

    EXPECT_CALL(*m_mockUart, recvBinary())
        .WillOnce(testing::Throw(std::runtime_error(errorMessage)));

    m_uut->start();
    m_uut->dataReady();
    m_uut->kill();
    m_uut->join();

    // Ensure we get the error message back
    CHECK(m_errorLogger->getString().find(errorMessage + "\n") != std::string::npos);
}
