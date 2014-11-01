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
        void invalidPacketFrame(uint8_t packetFrame) override;

    private:
        /**
         * \brief dumps all the bad data to hex form... unless
         *        it is in the alphanum range (0x21-0x7E) in the ascii table, 
         *        then it converts it to the out the ascii symbol.
         */
        static std::string dumpData(const std::vector<std::uint8_t> &badData);

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

        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

