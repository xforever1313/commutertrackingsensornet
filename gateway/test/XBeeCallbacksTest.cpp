#include <cstdint>
#include <string>
#include <vector>

#include "UnitTest.h"

#include "gateway/XBeeCallbacks.h"
#include "io/StringLogger.h"

TEST_GROUP(XBeeCallbacksTest) {
    TEST_SETUP() {
        m_outLogger = new Common::IO::StringLogger();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::XBeeCallbacks(*m_outLogger,*m_errLogger);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_outLogger;
        delete m_errLogger;
    }

    Common::IO::StringLogger *m_outLogger;
    Common::IO::StringLogger *m_errLogger;
    Gateway::XBeeCallbacks *m_uut;
};

TEST(XBeeCallbacksTest, dumpDataTest) {
    std::vector<std::uint8_t> data = {
        0x00,
        0x20,
        0x21,
        0x35,
        0x7e,
        0x7F
    };

    std::string expectedString = "[0x0, 0x20, !, 5, ~, 0x7f, ]";

    CHECK_EQUAL(Gateway::XBeeCallbacks::dumpData(data), expectedString);
}

TEST(XBeeCallbacksTest, successfulParseTest) {
    std::string s = "Hello :)";
    m_uut->successfulParse(s);

    CHECK(m_outLogger->getString().find(Gateway::XBeeCallbacks::SUCCESS_MESSAGE + s) 
          != std::string::npos);

    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(XBeeCallbacksTest, incompleteMessageTest) {
    std::vector<std::uint8_t> data = {
        0x00,
        0x20,
        0x21,
        0x35,
        0x7e,
        0x7F
    };

    m_uut->incompleteMessage(data);

    CHECK_EQUAL(m_outLogger->getString(), "");

    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::INCOMPLETE_MESSAGE +
                                        Gateway::XBeeCallbacks::dumpData(data)) 
          != std::string::npos);

}

TEST(XBeeCallbacksTest, badChecksumMessageTest) {
    std::vector<std::uint8_t> data = {
        0x00,
        0x20,
        0x21,
        0x35,
        0x7e,
        0x7F
    };

    m_uut->badChecksum(data);

    CHECK_EQUAL(m_outLogger->getString(), "");

    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::BAD_CHECKSUM_MESSAGE +
                                        Gateway::XBeeCallbacks::dumpData(data)) 
          != std::string::npos);

}

TEST(XBeeCallbacksTest, badStateTest) {
    std::vector<std::uint8_t> data = {
        0x00,
        0x20,
        0x21,
        0x35,
        0x7e,
        0x7F
    };

    m_uut->badState(data);

    CHECK_EQUAL(m_outLogger->getString(), "");

    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::BAD_STATE_MESSAGE +
                                        Gateway::XBeeCallbacks::dumpData(data)) 
          != std::string::npos);
}

TEST(XBeeCallbacksTest, hardwareResetTest) {
    m_uut->hardwareReset();

    CHECK(m_outLogger->getString().find(Gateway::XBeeCallbacks::HARDWARE_RESET_MESSAGE)
          != std::string::npos);

    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(XBeeCallbacksTest, watchdogTimerResetTest) {
    m_uut->watchdogTimerReset();

    CHECK(m_outLogger->getString().find(Gateway::XBeeCallbacks::WATCHDOG_TIMER_RESET_MESSAGE)
          != std::string::npos);

    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(XBeeCallbacksTest, networkWokeUpTest) {
    m_uut->networkWokeUp();

    CHECK(m_outLogger->getString().find(Gateway::XBeeCallbacks::NETWORK_WOKE_UP_MESSAGE)
          != std::string::npos);

    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(XBeeCallbacksTest, networkWentToSleepTest) {
    m_uut->networkWentToSleep();

    CHECK(m_outLogger->getString().find(Gateway::XBeeCallbacks::NETWORK_WENT_TO_SLEEP_MESSAGE)
          != std::string::npos);

    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(XBeeCallbacksTest, invalidModemStatusTest) {
    uint8_t badStatus = 0x90;
    m_uut->invalidModemStatus(badStatus);

    CHECK_EQUAL(m_outLogger->getString(), "");

    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::INVALID_MODEM_STATUS_MESSAGE +
                                        "0x90") 
          != std::string::npos);

}

TEST(XBeeCallbacksTest, badModemStatusPacketTest) {
    std::vector<std::uint8_t> data = {
        0x37,
        0x00,
        0x20,
        0x90,
        0x7F
    };

    m_uut->badModemStatusPacket(data);

    CHECK_EQUAL(m_outLogger->getString(), "");
    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::BAD_MODEM_STATUS_PACKET_MESSAGE +
                                        Gateway::XBeeCallbacks::dumpData(data))
          != std::string::npos);

}

