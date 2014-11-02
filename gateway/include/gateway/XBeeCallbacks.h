#ifndef XBEE_CALLBACKS_H_
#define XBEE_CALLBACKS_H_

#include <cstdint>
#include <string>
#include <vector>

#include "gateway/XBeeCallbackInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class XBeeCallbacks : public XBeeCallbackInterface {
    public:
         XBeeCallbacks(Common::IO::LoggerBase &outLogger = 
                           Common::IO::ConsoleLogger::out,
                       Common::IO::LoggerBase &errLogger = 
                          Common::IO::ConsoleLogger::err);

        ~XBeeCallbacks();

        void successfulParse(const std::string &payload) override;
        void incompleteMessage(const std::vector<std::uint8_t> &badData) override;
        void badChecksum(const std::vector<std::uint8_t> &badData) override;
        void badState(const std::vector<std::uint8_t> &badData) override;
        void hardwareReset() override;
        void watchdogTimerReset() override;
        void networkWokeUp() override;
        void networkWentToSleep() override;
        void invalidModemStatus(uint8_t badStatus) override;
        void badModemStatusPacket(const std::vector<std::uint8_t> &badData) override;
         void badTxStatusPacket(const std::vector<std::uint8_t> &badData) override;
        void invalidPacketFrame(uint8_t packetFrame) override;
        void transmitSuccess(uint8_t numAttempts, XBeeConstants::DiscoveryStatus discovery) override;
        void transmitFailure(uint8_t numAttempts, XBeeConstants::TxStatus errorNumber,
                             XBeeConstants::DiscoveryStatus discovery) override;
    private:
        /**
         * \brief dumps all the bad data to hex form... unless
         *        it is in the alphanum range (0x21-0x7E) in the ascii table, 
         *        then it converts it to the out the ascii symbol.
         */
        static std::string dumpData(const std::vector<std::uint8_t> &badData);

        static const std::string getDiscoveryString(XBeeConstants::DiscoveryStatus discovery);
        static const std::string getTxFailureReason(XBeeConstants::TxStatus txStatus);

        static const std::string SUCCESS_MESSAGE;
        static const std::string INCOMPLETE_MESSAGE;
        static const std::string BAD_CHECKSUM_MESSAGE;
        static const std::string BAD_STATE_MESSAGE;

        static const std::string HARDWARE_RESET_MESSAGE;
        static const std::string WATCHDOG_TIMER_RESET_MESSAGE;
        static const std::string NETWORK_WOKE_UP_MESSAGE;
        static const std::string NETWORK_WENT_TO_SLEEP_MESSAGE;
        static const std::string INVALID_MODEM_STATUS_MESSAGE;
        static const std::string BAD_MODEM_STATUS_PACKET_MESSAGE;
        static const std::string BAD_TX_STATUS_PACKET_MESSAGE;

        static const std::string TRANSMIT_SUCCESS_MESSAGE;
        static const std::string TRANSMIT_FAILURE_MESSAGE;

        static const std::string TRANSMIT_NO_DISCOVERY_OVERHEAD;
        static const std::string TRANSMIT_ROUTE_DISCOVERY_NEEDED;
        static const std::string TRANSMIT_UNKNOWN_ROUTE_DISCOVERY;

        static const std::string TX_STATUS_SUCCESS;
        static const std::string TX_STATUS_MAC_ACK_FAILURE;
        static const std::string TX_STATUS_INVALID_ENDPOINT;
        static const std::string TX_STATUS_NETWORK_ACK_FAILURE;
        static const std::string TX_STATUS_ROUTE_NOT_FOUND;
        static const std::string TX_STATUS_UNKNOWN;

        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

