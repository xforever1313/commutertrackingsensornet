#ifndef XBEE_CALLBACK_INTERFACE_H_
#define XBEE_CALLBACK_INTERFACE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "ctsn_common/XBeeConstants.h"

namespace CTSNCommon {

/**
 * \class XBeeCallbackInterface
 * \brief This class is an interface that is called when the XBee Controller
 *        is done parsing a packet.
 */
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

        /**
         * \brief Called if there's something structurally wrong with the
         *        Tx Status packet.
         */
        virtual void badTxStatusPacket(const std::vector<std::uint8_t> &badData) = 0;

        /**
         * \brief called if the packet frame is not a valid one.
         */
        virtual void invalidPacketFrame(uint8_t packetFrame) = 0;

        /**
         * \brief called when a tx success occurs.
         * \param numattempts the number of attempts the tx made.
         *        this is byte offset 7.
         * \param discovery whether or not there was discovery overhead
         *        or not.  byte offset 9
         */
        virtual void transmitSuccess(uint8_t numAttempts,
                                     CTSNCommon::XBeeConstants::DiscoveryStatus discovery) = 0;

        /**
         * \brief called when a tx failure occurs.
         * \param numattempts the number of attempts the tx made.
         *        this is byte offset 7.
         * \param errorNumber The error number from the tx status packet,
         *        byte offset 8
         * \param discovery whether or not there was discovery overhead
         *        or not.  byte offset 9
         */
        virtual void transmitFailure(uint8_t numAttempts,
                                     CTSNCommon::XBeeConstants::TxStatus errorNumber,
                                     CTSNCommon::XBeeConstants::DiscoveryStatus discovery) = 0;
};

}

#endif

