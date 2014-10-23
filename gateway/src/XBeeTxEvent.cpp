#include <cstdint>
#include <string>
#include <vector>

#include "gateway/Node.h"
#include "gateway/XBeeController.h"
#include "gateway/XBeeTxEvent.h"

namespace Gateway {

XBeeTxEvent::XBeeTxEvent(const Node &node,
                         const std::string &message,
                         UartInterface *uart,
                         Common::IO::LoggerBase &logger /*= Common::IO::ConsoleLogger::err*/) :
    m_node(node),
    m_message(message),
    m_uart(uart),
    m_errLogger(logger)
{

}

XBeeTxEvent::~XBeeTxEvent() {
}

void XBeeTxEvent::addStartChar() {
    m_packet.push_back(XBeeController::START_CHARACTER);
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
    uint8_t lsb = length;
    m_packet.push_back(msb);
    m_packet.push_back(lsb);
}

void XBeeTxEvent::addFrameInfo() {
    // Frame type - 0x10 as thats what the documentation says
    m_packet.push_back(0x10);

    // Frame id - 0x01 sends an ACK.  0x00 does not.
    m_packet.push_back(0x01);
}

void XBeeTxEvent::addAddress() {
    uint64_t address = m_node.getAddress();
    while(address) {
        // Get first two bytes
        uint8_t byte = (address & 0xFF00000000000000) >> 56;

        // shift the address over 1 byte to get the next byte
        address = address << 8;
        m_packet.push_back(byte);
    }
}

void XBeeTxEvent::addReservedBytes() {
    m_packet.push_back(0xFF);
    m_packet.push_back(0xFE);
}

void XBeeTxEvent::addTransmitOptions() {
    m_packet.push_back(0x00); // Default broadcast radius
    m_packet.push_back(0x00); // Use transmit options set on the device
}

void XBeeTxEvent::addPayload() {
    for (size_t i = 0; i < m_message.size(); ++i) {
        m_packet.push_back(static_cast<uint8_t>(m_message[i]));
    }
}

void XBeeTxEvent::addChecksum() {
    uint8_t checksum = 0;

    //Start at three to skip the start char and length
    for (size_t i = 3; i < m_packet.size(); ++i) {
        checksum += m_packet[i];
    }

    // Checksum is ff - the sum of everything after the length bytes.
    m_packet.push_back(0xff - checksum);
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

}

