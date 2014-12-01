#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <cstdint>
#include <vector>

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

        m_binData = {0x00, 0x01, 0x02};
        m_strData = "SomeString";
        m_uut = new Gateway::Uart(MockInterrupt);
        CHECK_EQUAL(m_uut->m_uartFile, Gateway::Uart::NOT_OPEN);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        MockLinux::readReturnBuffer = nullptr;
    }

    std::vector<std::uint8_t> m_binData;
    std::string m_strData;
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

///
/// String Tx Tests
///
TEST(PiUartTest, sendPass) {
    MockLinux::writeReturn = 0;
    try {
        m_uut->open("somefile");
        m_uut->send(m_strData);
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
        m_uut->send(m_strData);
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::SEND_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, sendFailNotOpened) {
    try {
        m_uut->send(m_strData);
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::NOT_OPEN_ERROR_MESSAGE);
    }
}

///
/// Tx Binary Tests
///
TEST(PiUartTest, sendBinPass) {
    MockLinux::writeReturn = 0;
    try {
        m_uut->open("somefile");
        m_uut->send(m_binData);
    }
    catch (const std::runtime_error &e) {
        std::string message = std::string("Unexpected Exception: ") + e.what();
        FAIL_TEST(message.c_str());
    }
}

TEST(PiUartTest, sendBinFailTxFail) {
    MockLinux::writeReturn = -1;
    try {
        m_uut->open("somefile");
        m_uut->send(m_binData);
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::SEND_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, sendBinFailNotOpened) {
    try {
        m_uut->send(m_binData);
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::NOT_OPEN_ERROR_MESSAGE);
    }
}

///
/// Recv String Tests
///
TEST(PiUartTest, recvPass) {
    MockLinux::readReturn = m_strData.size();
    MockLinux::readReturnBuffer = reinterpret_cast<void*>(const_cast<char*>(m_strData.data()));
    try {
        m_uut->open("somefile");
        std::string msg = m_uut->recvString();
        CHECK_EQUAL(msg, m_strData);
    }
    catch (const std::runtime_error &e) {
        std::string message = std::string("Unexpected Exception: ") + e.what();
        FAIL_TEST(message.c_str());
    }
}

TEST(PiUartTest, recvEmptyPass) {
    std::string s; // Empty string
    MockLinux::readReturn = s.size();
    MockLinux::readReturnBuffer = reinterpret_cast<void*>(const_cast<char*>(s.data()));
    try {
        m_uut->open("somefile");
        std::string msg = m_uut->recvString();
        CHECK_EQUAL(msg, s);
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
        std::string msg = m_uut->recvString();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::RECV_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, recvFailNotOpened) {
    try {
        std::string msg = m_uut->recvString();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::NOT_OPEN_ERROR_MESSAGE);
    }
}

///
/// recvBinaryTests
///
TEST(PiUartTest, recvBinPass) {
    MockLinux::readReturn = m_binData.size();
    MockLinux::readReturnBuffer = m_binData.data();

    try {
        m_uut->open("somefile");
        auto msg = m_uut->recvBinary();
        CHECK_EQUAL(msg.size(), m_binData.size());
        CHECK_EQUAL(msg[0], m_binData[0]);
        CHECK_EQUAL(msg[1], m_binData[1]);
        CHECK_EQUAL(msg[2], m_binData[2]);
    }
    catch (const std::runtime_error &e) {
        std::string message = std::string("Unexpected Exception: ") + e.what();
        FAIL_TEST(message.c_str());
    }
}

TEST(PiUartTest, recvBinPassReturnEmpty) {
    MockLinux::readReturn = 0; //Return empty data
    MockLinux::readReturnBuffer = m_binData.data();

    try {
        m_uut->open("somefile");
        auto msg = m_uut->recvBinary();
        CHECK(msg.empty());
    }
    catch (const std::runtime_error &e) {
        std::string message = std::string("Unexpected Exception: ") + e.what();
        FAIL_TEST(message.c_str());
    }
}

TEST(PiUartTest, recvBinFailRxFail) {
    MockLinux::readReturn = -1;
    try {
        m_uut->open("somefile");
        auto msg = m_uut->recvBinary();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::RECV_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, recvBinFailNotOpened) {
    try {
        auto msg = m_uut->recvBinary();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::Uart::NOT_OPEN_ERROR_MESSAGE);
    }
}

///
/// Close tests
///
TEST(PiUartTest, closeTest) {
    m_uut->m_uartFile = 0;

    m_uut->close();

    CHECK_EQUAL(m_uut->m_uartFile, Gateway::Uart::NOT_OPEN);
}

