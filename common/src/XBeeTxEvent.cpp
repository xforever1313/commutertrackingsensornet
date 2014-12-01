#include <cstdint>
#include <string>
#include <vector>

#include "ctsn_common/Node.h"
#include "ctsn_common/XBeeConstants.h"
#include "ctsn_common/XBeeTxEvent.h"

namespace CTSNCommon {

XBeeTxEvent::XBeeTxEvent(const Node &node,
                         const std::string &message,
                         UartInterface *uart,
                         Common::IO::LoggerBase &logger /*= Common::IO::ConsoleLogger::err*/) :
    m_node(node),
    m_message(message),
    m_uart(uart),
    m_errLogger(logger),
    m_checksum(0)
{

}

XBeeTxEvent::~XBeeTxEvent() {
}

void XBeeTxEvent::addStartChar() {
    m_packet.push_back(XBeeConstants::START_CHARACTER);
}

void XBeeTxEvent::addLength() {
    // 14 is:
    // 1 for frame type
    // 1 for frame id
    // 8 for address
    // 2 for reserved
    // 2 for options
    uint16_t length = m_message.size() + 14;
    uint8_t msb = length >> 8;
    if (isEscapedCharacter(msb)) {
        m_packet.push_back(XBeeConstants::ESCAPE_CHARACTER);
        msb = msb ^ XBeeConstants::ESCAPE_XOR;
    }
    m_packet.push_back(msb);

    uint8_t lsb = length;
    if (isEscapedCharacter(lsb)) {
        m_packet.push_back(XBeeConstants::ESCAPE_CHARACTER);
        lsb = lsb ^ XBeeConstants::ESCAPE_XOR;
    }
    m_packet.push_back(lsb);
}

void XBeeTxEvent::addFrameInfo() {
    // Frame type - 0x10 as thats what the documentation says
    const uint8_t frameType = 0x10;
    m_packet.push_back(frameType);
    m_checksum += frameType;

    // Frame id - 0x01 sends an ACK.  0x00 does not.
    const uint8_t frameID = 0x01;
    m_packet.push_back(frameID);
    m_checksum += frameID;
}

void XBeeTxEvent::addAddress() {
    uint64_t address = m_node.getAddress();
    while(address) {
        // Get first two bytes
        uint8_t byte = (address & 0xFF00000000000000) >> 56;
        m_checksum += byte;
        if (isEscapedCharacter(byte)) {
            //Escaped characters must be xored with 0x02
            byte = byte ^ XBeeConstants::ESCAPE_XOR;

            m_packet.push_back(XBeeConstants::ESCAPE_CHARACTER);
        }

        // shift the address over 1 byte to get the next byte
        address = address << 8;
        m_packet.push_back(byte);
    }
}

void XBeeTxEvent::addReservedBytes() {
    const uint8_t reserved1 = 0xFF;
    m_packet.push_back(reserved1);
    m_checksum += reserved1;

    const uint8_t reserved2 = 0xFE;
    m_packet.push_back(reserved2);
    m_checksum += reserved2;
}

void XBeeTxEvent::addTransmitOptions() {
    const uint8_t broadCastRadius = 0x00; // Default broadcast radius
    m_packet.push_back(broadCastRadius);
    m_checksum += broadCastRadius;

    const uint8_t transmitOptions = 0x00; // Use transmit options set on the device

    m_packet.push_back(transmitOptions);
    m_checksum += transmitOptions;
}

void XBeeTxEvent::addPayload() {
    for (size_t i = 0; i < m_message.size(); ++i) {
        uint8_t ch = m_message[i];
        m_checksum += ch;
        if (isEscapedCharacter(ch)) {
            ch = ch ^ XBeeConstants::ESCAPE_XOR;
            m_packet.push_back(XBeeConstants::ESCAPE_CHARACTER);
        }
        m_packet.push_back(ch);
    }
}

void XBeeTxEvent::addChecksum() {
    // Checksum is ff - the sum of everything after the length bytes.
    m_packet.push_back(0xff - m_checksum);
}

void XBeeTxEvent::execute() {
    try {
        addStartChar();
        addLength();
        addFrameInfo();
        addAddress();
        addReservedBytes();
        addTransmitOptions();
        addPayload();
        addChecksum();

        m_uart->send(m_packet);
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

bool XBeeTxEvent::isEscapedCharacter(uint8_t c) const {
    bool ret = false;
    if ((c == XBeeConstants::START_CHARACTER) ||
        (c == XBeeConstants::ESCAPE_CHARACTER) ||
        (c == XBeeConstants::XON) ||
        (c == XBeeConstants::XOFF)) {

        ret = true;
    }

    return ret;
}

}

