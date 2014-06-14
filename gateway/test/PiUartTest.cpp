#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "MockLinux.h"
#include "gateway/PiUart.h"

TEST_GROUP(PiUartTest) {
    TEST_SETUP() {
        MockLinux::openReturn = 0;
        MockLinux::readReturn = 0;
        MockLinux::writeReturn = 0;
        MockLinux::closeReturn = 0;

        m_uut = new Gateway::PiUart();
        CHECK_EQUAL(m_uut->m_uartFile, Gateway::PiUart::NOT_OPEN);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    Gateway::PiUart *m_uut;
};

TEST(PiUartTest, openTestPass) {
    int expected = 1;
    MockLinux::openReturn = expected;
    m_uut->open("somefile");
    CHECK_EQUAL(m_uut->m_uartFile, expected);
}

TEST(PiUartTest, openFail) {
    MockLinux::openReturn = Gateway::PiUart::NOT_OPEN;
    try {
        m_uut->open("somefile");
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::PiUart::OPEN_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, sendFailNotOpened) {
    try {
        m_uut->send("somestr");
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::PiUart::NOT_OPEN_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, recvFailNotOpened) {
    try {
        std::string msg = m_uut->recv();
        FAIL_TEST("Exception not thrown");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(std::string(e.what()), Gateway::PiUart::NOT_OPEN_ERROR_MESSAGE);
    }
}

TEST(PiUartTest, closeTest) {
    m_uut->m_uartFile = 0;

    m_uut->close();

    CHECK_EQUAL(m_uut->m_uartFile, Gateway::PiUart::NOT_OPEN);
}
