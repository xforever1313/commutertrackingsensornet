#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "MockLinux.h"
#include "gateway/Uart.h"

void MockInterrupt(int) {

}

TEST_GROUP(PiUartTest) {
    TEST_SETUP() {
        MockLinux::openReturn = 0;
        MockLinux::readReturn = 0;
        MockLinux::writeReturn = 0;
        MockLinux::closeReturn = 0;

        m_uut = new Gateway::Uart(MockInterrupt);
        CHECK_EQUAL(m_uut->m_uartFile, Gateway::Uart::NOT_OPEN);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    Gateway::Uart *m_uut;
};

TEST(PiUartTest, openTestPass) {
    int expected = 1;
    MockLinux::openReturn = expected;
    m_uut->open("somefile");
    CHECK_EQUAL(m_uut->m_uartFile, expected);
}

TEST(PiUartTest, openFail) {
    MockLinux::openReturn = Gateway::Uart::NOT_OPEN;
    try {
        m_uut->open("somefile");
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::OPEN_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, sendPass) {
    MockLinux::writeReturn = 0;
    try {
        m_uut->open("somefile");
        m_uut->send("somestr");
    }
    catch (const std::runtime_error &e) {
        std::string message = std::string("Unexpected Exception: ") + e.what();
        FAIL_TEST(message.c_str());
    }
}

TEST(PiUartTest, sendFailTxFail) {
    MockLinux::writeReturn = -1;
    try {
        m_uut->open("somefile");
        m_uut->send("somestr");
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::SEND_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, sendFailNotOpened) {
    try {
        m_uut->send("somestr");
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::NOT_OPEN_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, recvPass) {
    MockLinux::readReturn = 0;
    try {
        m_uut->open("somefile");
        std::string msg = m_uut->recv();
        CHECK_EQUAL(msg, "");
    }
    catch (const std::runtime_error &e) {
        std::string message = std::string("Unexpected Exception: ") + e.what();
        FAIL_TEST(message.c_str());
    }
}

TEST(PiUartTest, recvFailRxFail) {
    MockLinux::readReturn = -1;
    try {
        m_uut->open("somefile");
        std::string msg = m_uut->recv();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::RECV_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, recvFailNotOpened) {
    try {
        std::string msg = m_uut->recv();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::NOT_OPEN_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, closeTest) {
    m_uut->m_uartFile = 0;

    m_uut->close();

    CHECK_EQUAL(m_uut->m_uartFile, Gateway::Uart::NOT_OPEN);
}
