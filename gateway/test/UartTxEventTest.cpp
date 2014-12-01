#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "ctsn_common/Uart.h"
#include "gateway/UartTxEvent.h"
#include "io/StringLogger.h"
#include "MockUart.h"

TEST_GROUP(UartTxEventTest) {
    TEST_SETUP() {
        m_message = "Message to send";
        m_errorMessage = "ERROR!!!!";
        m_logger = new Common::IO::StringLogger();
        m_mockUart = new testing::StrictMock<CTSNCommon::MockUart>();
        m_uut = new Gateway::UartTxEvent(m_message, m_mockUart, *m_logger);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mockUart;
        delete m_logger;
    }

    std::string m_message;
    std::string m_errorMessage;
    Common::IO::StringLogger *m_logger;
    testing::StrictMock<CTSNCommon::MockUart> *m_mockUart;
    Gateway::UartTxEvent *m_uut;
};

TEST(UartTxEventTest, TxSuccess) {
    EXPECT_CALL(*m_mockUart, send(m_message));
    m_uut->execute();
}

TEST(UartTxEventTest, TxFail) {
    EXPECT_CALL(*m_mockUart, send(m_message))
        .WillOnce(testing::Throw(std::runtime_error(m_errorMessage)));

    m_uut->execute();

    //Ensure the error message gets printed to the log.
    CHECK_EQUAL(m_logger->getString(), m_errorMessage + "\n");
}
