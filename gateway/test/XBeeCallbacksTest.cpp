#include <cstdint>
#include <string>
#include <vector>

#include "UnitTest.h"

#include "CTSNSharedGlobals.py"
#include "gateway/XBeeCallbacks.h"
#include "ctsn_common/XBeeConstants.h"
#include "io/StringLogger.h"
#include "MockHTTPPoster.h"

TEST_GROUP(XBeeCallbacksTest) {
    TEST_SETUP() {
        m_outLogger = new Common::IO::StringLogger();
        m_errLogger = new Common::IO::StringLogger();
        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();
        m_uut = new Gateway::XBeeCallbacks(*m_outLogger,*m_errLogger);

        // Replace real poster with mock one.
        delete m_uut->m_poster;
        m_uut->m_poster = m_httpPoster;
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_outLogger;
        delete m_errLogger;

        // http poster is deleted in uut
    }

    Common::IO::StringLogger *m_outLogger;
    Common::IO::StringLogger *m_errLogger;
    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;
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

/////
/// Successful parse tests
/////
TEST(XBeeCallbacksTest, successfulParseTest) {
    std::string s = "/shutdown\tshutdown=true|derp=herp";

    EXPECT_CALL(*m_httpPoster, post("localhost",
                                    "/shutdown", 
                                    "shutdown=true&derp=herp",
                                    GATEWAY_COMMAND_PORT));

    m_uut->successfulParse(s);

    CHECK_EQUAL(m_outLogger->getString(), "");
    CHECK_EQUAL(m_errLogger->getString(), "");
}

TEST(XBeeCallbacksTest, curlFailTest) {
    const std::string error = "error";
    const std::string s = "/shutdown\tshutdown=true|something=Something";

    EXPECT_CALL(*m_httpPoster, post("localhost",
                                    "/shutdown",
                                    "shutdown=true&something=Something",
                                    GATEWAY_COMMAND_PORT))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->successfulParse(s);

    CHECK_EQUAL(m_outLogger->getString(), "");
    CHECK(m_errLogger->getString().find(error + "Payload:\n\t" + s) != std::string::npos);
}

TEST(XBeeCallbacksTest, noTabs) {
    const std::string s = "/shutdownshutdown=true";

    m_uut->successfulParse(s);

    CHECK_EQUAL(m_outLogger->getString(), "");
    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::BAD_PAYLOAD + "Payload:\n\t" + s)        
        != std::string::npos);
}

TEST(XBeeCallbacksTest, tooManyTabs) {
    const std::string s = "/shutdown\t\tshutdown=true";

    m_uut->successfulParse(s);

    CHECK_EQUAL(m_outLogger->getString(), "");
    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::BAD_PAYLOAD + "Payload:\n\t" + s)        
        != std::string::npos);
}

/////
/// Fail cases
/////
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

TEST(XBeeCallbacksTest, badTxStatusPacketTest) {
    std::vector<std::uint8_t> data = {
        0x37,
        0x00,
        0x20,
        0x90,
        0x7F
    };

    m_uut->badTxStatusPacket(data);

    CHECK_EQUAL(m_outLogger->getString(), "");
    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::BAD_TX_STATUS_PACKET_MESSAGE +
                                        Gateway::XBeeCallbacks::dumpData(data))
          != std::string::npos);
}

TEST(XBeeCallbacksTest, invalidPacketFrameTest) {
    uint8_t badPacket = 0x90;
    m_uut->invalidPacketFrame(badPacket);

    CHECK_EQUAL(m_outLogger->getString(), "");

    CHECK(m_errLogger->getString().find(Gateway::XBeeCallbacks::INVALID_MODEM_STATUS_MESSAGE +
                                        "0x90") 
          != std::string::npos);

}

TEST(XBeeCallbacksTest, tramsmitSuccess) {
    uint8_t numAttempts = 0x01;
    CTSNCommon::XBeeConstants::DiscoveryStatus discovery = CTSNCommon::XBeeConstants::DiscoveryStatus::NO_OVERHEAD;

    std::string expectedString = Gateway::XBeeCallbacks::TRANSMIT_SUCCESS_MESSAGE +
                                 "Attempts: 1\n\tDiscovery: " + 
                                 Gateway::XBeeCallbacks::getDiscoveryString(discovery);

    m_uut->transmitSuccess(numAttempts, discovery);

    CHECK(m_outLogger->getString().find(expectedString)
          != std::string::npos);

    CHECK_EQUAL(m_errLogger->getString(), "");

}

TEST(XBeeCallbacksTest, tramsmitFailure) {
    uint8_t numAttempts = 0x01;
    CTSNCommon::XBeeConstants::TxStatus status = CTSNCommon::XBeeConstants::TxStatus::ROUTE_NOT_FOUND;
    CTSNCommon::XBeeConstants::DiscoveryStatus discovery = CTSNCommon::XBeeConstants::DiscoveryStatus::NO_OVERHEAD;

    std::string expectedString = Gateway::XBeeCallbacks::TRANSMIT_FAILURE_MESSAGE +
                                 "Attempts: 1\n\tDiscovery: " + 
                                 Gateway::XBeeCallbacks::getDiscoveryString(discovery) +
                                 "\n\tReason: " +
                                 Gateway::XBeeCallbacks::getTxFailureReason(status);

    m_uut->transmitFailure(numAttempts, status, discovery);

    CHECK_EQUAL(m_outLogger->getString(), "");

    CHECK(m_errLogger->getString().find(expectedString)
          != std::string::npos);
}


TEST(XBeeCallbacksTest, getDiscoveryStringTest) {
    const std::string s1 = 
        Gateway::XBeeCallbacks::getDiscoveryString(CTSNCommon::XBeeConstants::DiscoveryStatus::UNKNOWN_DISCOVERY_STATUS);

    const std::string s2 = 
        Gateway::XBeeCallbacks::getDiscoveryString(CTSNCommon::XBeeConstants::DiscoveryStatus::NO_OVERHEAD);

    const std::string s3 = 
        Gateway::XBeeCallbacks::getDiscoveryString(CTSNCommon::XBeeConstants::DiscoveryStatus::ROUTE_DISCOVERY);

    CHECK_EQUAL(s1, Gateway::XBeeCallbacks::TRANSMIT_UNKNOWN_ROUTE_DISCOVERY);
    CHECK_EQUAL(s2, Gateway::XBeeCallbacks::TRANSMIT_NO_DISCOVERY_OVERHEAD);
    CHECK_EQUAL(s3, Gateway::XBeeCallbacks::TRANSMIT_ROUTE_DISCOVERY_NEEDED);
}

TEST(XBeeCallbacksTest, getTxFailureReasonTest) {
    const std::string s1 = 
        Gateway::XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus::UNKNOWN_TX_STATUS);

    const std::string s2 = 
        Gateway::XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus::SUCCESS);

    const std::string s3 = 
        Gateway::XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus::MAC_ACK_FAILURE);

    const std::string s4 = 
        Gateway::XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus::INVALID_ENDPOINT);

    const std::string s5 = 
        Gateway::XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus::NETWORK_ACK_FAILURE);

    const std::string s6 = 
        Gateway::XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus::ROUTE_NOT_FOUND);

    CHECK_EQUAL(s1, Gateway::XBeeCallbacks::TX_STATUS_UNKNOWN);
    CHECK_EQUAL(s2, Gateway::XBeeCallbacks::TX_STATUS_SUCCESS);
    CHECK_EQUAL(s3, Gateway::XBeeCallbacks::TX_STATUS_MAC_ACK_FAILURE);
    CHECK_EQUAL(s4, Gateway::XBeeCallbacks::TX_STATUS_INVALID_ENDPOINT);
    CHECK_EQUAL(s5, Gateway::XBeeCallbacks::TX_STATUS_NETWORK_ACK_FAILURE);
    CHECK_EQUAL(s6, Gateway::XBeeCallbacks::TX_STATUS_ROUTE_NOT_FOUND);
}

