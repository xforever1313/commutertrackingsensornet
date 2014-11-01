#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "gateway/XBeeCallbacks.h"
#include "io/LoggerBase.h"

namespace Gateway {

const std::string XBeeCallbacks::SUCCESS_MESSAGE = "XBee Message -\n\t";
const std::string XBeeCallbacks::INCOMPLETE_MESSAGE = "Incomplete XBee Message -\n\t";
const std::string XBeeCallbacks::BAD_CHECKSUM_MESSAGE = "Bad XBee Checksum -\n\t";
const std::string XBeeCallbacks::BAD_STATE_MESSAGE = "Bad XBee Controller State -\n\t";
const std::string XBeeCallbacks::HARDWARE_RESET_MESSAGE = "XBee Hardware Reset";
const std::string XBeeCallbacks::WATCHDOG_TIMER_RESET_MESSAGE = "XBee Watchdog Timer Reset";
const std::string XBeeCallbacks::NETWORK_WOKE_UP_MESSAGE = "XBee network woke up";
const std::string XBeeCallbacks::NETWORK_WENT_TO_SLEEP_MESSAGE = "XBee network went to sleep";
const std::string XBeeCallbacks::INVALID_MODEM_STATUS_MESSAGE = "XBee invalid modem status - ";
const std::string XBeeCallbacks::BAD_MODEM_STATUS_PACKET_MESSAGE = "XBee invalid modem packet -\n\t";

XBeeCallbacks::XBeeCallbacks(Common::IO::LoggerBase &outLogger,/* = Common::IO::ConsoleLogger::out */
                             Common::IO::LoggerBase &errLogger/* = Common::IO::ConsoleLogger::err */) :
    m_outLogger(outLogger),
    m_errLogger(errLogger)
{
}

XBeeCallbacks::~XBeeCallbacks() {

}

void XBeeCallbacks::successfulParse(const std::string &payload) {
    m_outLogger.writeLineWithTimeStamp(SUCCESS_MESSAGE + payload);
}

void XBeeCallbacks::incompleteMessage(const std::vector<std::uint8_t> &badData) {
    m_errLogger.writeLineWithTimeStamp(INCOMPLETE_MESSAGE + dumpData(badData));
}

void XBeeCallbacks::badChecksum(const std::vector<std::uint8_t> &badData) {
     m_errLogger.writeLineWithTimeStamp(BAD_CHECKSUM_MESSAGE + dumpData(badData));
}

void XBeeCallbacks::badState(const std::vector<std::uint8_t> &badData) {
     m_errLogger.writeLineWithTimeStamp(BAD_STATE_MESSAGE + dumpData(badData));
}

void XBeeCallbacks::hardwareReset() {
    m_outLogger.writeLineWithTimeStamp(HARDWARE_RESET_MESSAGE);
}

void XBeeCallbacks::watchdogTimerReset() {
    m_outLogger.writeLineWithTimeStamp(WATCHDOG_TIMER_RESET_MESSAGE);
}

void XBeeCallbacks::networkWokeUp() {
    m_outLogger.writeLineWithTimeStamp(NETWORK_WOKE_UP_MESSAGE);
}

void XBeeCallbacks::networkWentToSleep() {
    m_outLogger.writeLineWithTimeStamp(NETWORK_WENT_TO_SLEEP_MESSAGE);
}

void XBeeCallbacks::invalidModemStatus(uint8_t badStatus) {
    std::stringstream ss;
    ss << std::hex << INVALID_MODEM_STATUS_MESSAGE << "0x" << static_cast<unsigned short>(badStatus);
    m_errLogger.writeLineWithTimeStamp(ss.str());
}

void XBeeCallbacks::badModemStatusPacket(const std::vector<std::uint8_t> &badData) {
    m_errLogger.writeLineWithTimeStamp(BAD_MODEM_STATUS_PACKET_MESSAGE + dumpData(badData));
}

void XBeeCallbacks::invalidPacketFrame(uint8_t packetFrame) {
    std::stringstream ss;
    ss << std::hex << INVALID_MODEM_STATUS_MESSAGE << "0x" << static_cast<unsigned short>(packetFrame);
    m_errLogger.writeLineWithTimeStamp(ss.str());
}

std::string XBeeCallbacks::dumpData(const std::vector<std::uint8_t> &badData) {
    std::stringstream ss;
    ss << "[" << std::hex;
    for (size_t i = 0; i < badData.size(); ++i) {
        if ((badData[i] <= 0x20) || (badData[i] >= 0x7F)) {
           // Convert to a short.  an unsigned char will be treated like a char,
           // So if 0x00 comes along, it will think its a null character.
           ss << "0x" << static_cast<unsigned short>(badData[i]);
        }
        else {
            ss << static_cast<char>(badData[i]);
        }
        ss << ", ";
    }

    ss << "]";

    return ss.str();
}

}

