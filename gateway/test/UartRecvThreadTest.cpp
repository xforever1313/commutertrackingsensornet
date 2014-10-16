#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <stdexcept>

#define private public
#define protected public

#include "gateway/UartRecvThread.h"
#include "io/StringLogger.h"
#include "MockUart.h"

TEST_GROUP(UartRecvThreadTest) {

    TEST_SETUP() {
        m_outputLogger = new Common::IO::StringLogger();
        m_errorLogger = new Common::IO::StringLogger();
        m_mockUart = new testing::StrictMock<Gateway::MockUart>();
        m_uut = new Gateway::UartRecvThread(m_mockUart, *m_outputLogger,
                                            *m_errorLogger);

        CHECK(m_uut->m_isAlive);
        CHECK(m_uut->isAlive());
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mockUart;
        delete m_errorLogger;
        delete m_outputLogger;
    }

    Common::IO::StringLogger *m_outputLogger;
    Common::IO::StringLogger *m_errorLogger;
    testing::StrictMock<Gateway::MockUart> *m_mockUart;
    Gateway::UartRecvThread *m_uut;
};

TEST(UartRecvThreadTest, killTest) {
    m_uut->kill();

    CHECK(!m_uut->m_isAlive);
    CHECK(!m_uut->isAlive());
}

TEST(UartRecvThreadTest, runSuccessTest) {
    std::string str1 = "Hello ";

    testing::InSequence dummy;

    EXPECT_CALL(*m_mockUart, recvString())
        .WillOnce(testing::Return(str1));

    m_uut->start();
    m_uut->dataReady();
    m_uut->kill();
    m_uut->join();

    // Ensure we got the right message
    CHECK_EQUAL(m_outputLogger->getString(), Gateway::UartRecvThread::MESSAGE_PREFIX + \
                                             str1 + "\n");

    // Ensure no errors
    CHECK_EQUAL(m_errorLogger->getString(), "");
}

TEST(UartRecvThreadTest, runFailTest) {
    std::string errorMessage = "Hello ";

    testing::InSequence dummy;

    EXPECT_CALL(*m_mockUart, recvString())
        .WillOnce(testing::Throw(std::runtime_error(errorMessage)));

    m_uut->start();
    m_uut->dataReady();
    m_uut->kill();
    m_uut->join();

    // Ensure nothing was printed to stdout
    CHECK_EQUAL(m_outputLogger->getString(), "");

    // Ensure we get the error message back
    CHECK_EQUAL(m_errorLogger->getString(), Gateway::UartRecvThread::MESSAGE_PREFIX + \
                                            errorMessage + "\n");
}
