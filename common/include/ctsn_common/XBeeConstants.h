#ifndef XBEE_CONSTANTS_H_
#define XBEE_CONSTANTS_H_

#include <cstdint>

namespace CTSNCommon {

/**
 * \class XBeeConstants
 * \brief Constants needed for XBee parsing.
 */
class XBeeConstants {
    public:
        /**
         * \brief the different types of packet frames
         * \note not all of them are used in this program, they
         *       are here for reference
         */
        enum PacketFrame {
            UNKNOWN = 0x00,
            AT_COMMAND = 0x08,
            QUEUE_PARAMETER = 0x09,
            TRANSMIT_REQUEST = 0x10,
            EXPLICIT_ADDRESSING = 0x11,
            REMOTE_COMMAND_REQUEST = 0x17,
            AT_COMMAND_RESPONSE = 0x88,
            MODEM_STATUS = 0x8A,
            TRANSMIT_STATUS = 0x8B,
            RECEIVE_PACKET = 0x90,
            EXPLICIT_RX_INDICATIOR = 0x91,
            NODE_IDENTIFICATION_INDICATIOR = 0x95,
            REMOTE_COMMAND_RESPONSE = 0x97
        };

        enum ModemStatus {
            UNKNOWN_STATUS = 0xff,
            HARDWARE_RESET = 0x00,
            WATCHDOG_TIMER_RESET = 0x01,
            NETWORK_WENT_TO_SLEEP = 0x0B,
            NETWORK_WOKE_UP = 0x0C
        };

        enum TxStatus {
            UNKNOWN_TX_STATUS = 0xff,
            SUCCESS = 0x00,
            MAC_ACK_FAILURE = 0x01,
            INVALID_ENDPOINT = 0x15,
            NETWORK_ACK_FAILURE = 0x21,
            ROUTE_NOT_FOUND = 0x25
        };

        enum DiscoveryStatus {
            UNKNOWN_DISCOVERY_STATUS = 0xff,
            NO_OVERHEAD = 0x00,
            ROUTE_DISCOVERY = 0x02
        };

        static const uint8_t START_CHARACTER;
        static const uint8_t ESCAPE_CHARACTER;
        static const uint8_t XON;
        static const uint8_t XOFF;
        static const uint8_t ESCAPE_XOR;

    private:
        XBeeConstants() = delete;
        XBeeConstants(const XBeeConstants&) = delete;
};

}

#endif

