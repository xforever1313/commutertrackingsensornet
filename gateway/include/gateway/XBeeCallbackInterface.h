#ifndef XBEE_CALLBACK_INTERFACE_H_
#define XBEE_CALLBACK_INTERFACE_H_

#include <cstdint>
#include <string>
#include <vector>

namespace Gateway {

class XBeeCallbackInterface {
    public:
        virtual ~XBeeCallbackInterface(){}

        /**
         * \brief called when the parse is successful
         * \param payLoad the string from the message
         */
        virtual void successfulParse(const std::string &payload) = 0;

        /**
         * \brief called if a message is incomplete, that is the start character appears 
         *        before it was expected.
         * \param badData The data that caused the bad message.
         */
        virtual void incompleteMessage(const std::vector<std::uint8_t> &badData) = 0;

        /**
         * \brief Called if the checksum does not match
         * \param badData The data that caused the bad message
         */
        virtual void badChecksum(const std::vector<std::uint8_t> &badData) = 0;

        /**
         * \brief Called if the state somehow ends up bad
         * \param badData The data that may have caused the bad message
         */
        virtual void badState(const std::vector<std::uint8_t> &badData) = 0;

        /**
         * \brief Called if the modem status returns 0x00 (A Hardware Reset)
         */
        virtual void hardwareReset() = 0;

        /**
         * \brief called if the modem status returns 0x01
         *        (Watchdog timer reset)
         */
        virtual void watchdogTimerReset() = 0;

        /**
         * \brief Called if the modem status returns 0x0b
         *        (Network woke up)
         */
        virtual void networkWokeUp() = 0;

        /**
         * \brief Called if the modem status returns 0x0c
         *        (Network went to sleep)
         */
        virtual void networkWentToSleep() = 0;

        /**
         * \brief called if the modem status is not valid.
         */
        virtual void invalidModemStatus(uint8_t badStatus) = 0;

        /**
         * \brief called if there's something structurally wrong with the 
         *        modem status packet.
         */
        virtual void badModemStatusPacket(const std::vector<std::uint8_t> &badData) = 0;
};

}

#endif

