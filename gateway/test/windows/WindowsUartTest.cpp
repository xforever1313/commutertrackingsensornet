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
        m_uut = new Gateway::Uart(MockInterrupt);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    Gateway::Uart *m_uut;
};

