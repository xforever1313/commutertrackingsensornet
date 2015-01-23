#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "CTSNSharedGlobals.py"
#include "ctsn_common/HTTPPoster.h"
#include "ctsn_common/XBeeCallbacks.h"
#include "ctsn_common/XBeeConstants.h"
#include "io/LoggerBase.h"
#include "StringOps.h"

namespace CTSNCommon {

const char XBeeCallbacks::DATA_SEPARATOR = '\t';
const char XBeeCallbacks::AMP_REPLACE = '|';
const std::string XBeeCallbacks::BAD_PAYLOAD = "Bad payload.  Missing a tab or has too many - ";
const std::string XBeeCallbacks::INCOMPLETE_MESSAGE = "Incomplete XBee Message -\n\t";
const std::string XBeeCallbacks::BAD_CHECKSUM_MESSAGE = "Bad XBee Checksum -\n\t";
const std::string XBeeCallbacks::BAD_STATE_MESSAGE = "Bad XBee Controller State -\n\t";
const std::string XBeeCallbacks::HARDWARE_RESET_MESSAGE = "XBee Hardware Reset";
const std::string XBeeCallbacks::WATCHDOG_TIMER_RESET_MESSAGE = "XBee Watchdog Timer Reset";
const std::string XBeeCallbacks::NETWORK_WOKE_UP_MESSAGE = "XBee network woke up";
const std::string XBeeCallbacks::NETWORK_WENT_TO_SLEEP_MESSAGE = "XBee network went to sleep";
const std::string XBeeCallbacks::INVALID_MODEM_STATUS_MESSAGE = "XBee invalid modem status - ";
const std::string XBeeCallbacks::BAD_MODEM_STATUS_PACKET_MESSAGE = "XBee invalid modem packet -\n\t";
const std::string XBeeCallbacks::BAD_TX_STATUS_PACKET_MESSAGE = "XBee invalid tx status packet - \n\t";

const std::string XBeeCallbacks::TRANSMIT_SUCCESS_MESSAGE = "Tx Success - \n\t";
const std::string XBeeCallbacks::TRANSMIT_FAILURE_MESSAGE = "Tx Failure - \n\t";

const std::string XBeeCallbacks::TRANSMIT_NO_DISCOVERY_OVERHEAD = "No discovery overhead.";
const std::string XBeeCallbacks::TRANSMIT_ROUTE_DISCOVERY_NEEDED = "Route discovery needed.";
const std::string XBeeCallbacks::TRANSMIT_UNKNOWN_ROUTE_DISCOVERY = "Unknown route discovery message";

const std::string XBeeCallbacks::TX_STATUS_SUCCESS = "Tx Success";
const std::string XBeeCallbacks::TX_STATUS_MAC_ACK_FAILURE = "Mac Ack Failure";
const std::string XBeeCallbacks::TX_STATUS_INVALID_ENDPOINT = "Invalid Endpoint";
const std::string XBeeCallbacks::TX_STATUS_NETWORK_ACK_FAILURE = "Network Ack Failure";
const std::string XBeeCallbacks::TX_STATUS_ROUTE_NOT_FOUND = "Route Not Found";
const std::string XBeeCallbacks::TX_STATUS_UNKNOWN = "Unknown Tx Error";

XBeeCallbacks::XBeeCallbacks(Common::IO::LoggerBase &outLogger,/* = Common::IO::ConsoleLogger::out */
                             Common::IO::LoggerBase &errLogger/* = Common::IO::ConsoleLogger::err */) :
    m_outLogger(outLogger),
    m_errLogger(errLogger),
    m_poster(new CTSNCommon::HTTPPoster())
{
}

XBeeCallbacks::~XBeeCallbacks() {
    delete m_poster;
}

void XBeeCallbacks::successfulParse(const std::string &payload) {
    try {
        std::vector<std::string> data =
            Common::StringOps::split(payload, DATA_SEPARATOR);

        // If there are no tabs or too many, bad payload.  Throw.
        if (data.size() != 2) {
            throw std::runtime_error(BAD_PAYLOAD);
        }

        for (size_t i = 0; i < data[1].size(); ++i) {
            if (data[1][i] == AMP_REPLACE) {
                data[1][i] = '&';
            }
        }

        m_poster->post("localhost", data[0], data[1], GATEWAY_COMMAND_PORT);
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(std::string(e.what()) +
                                           "Payload:\n\t" + payload);
    }
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

void XBeeCallbacks::badTxStatusPacket(const std::vector<std::uint8_t> &badData) {
    m_errLogger.writeLineWithTimeStamp(BAD_TX_STATUS_PACKET_MESSAGE + dumpData(badData));
}

void XBeeCallbacks::invalidPacketFrame(uint8_t packetFrame) {
    std::stringstream ss;
    ss << std::hex << INVALID_MODEM_STATUS_MESSAGE << "0x" << static_cast<unsigned short>(packetFrame);
    m_errLogger.writeLineWithTimeStamp(ss.str());
}

void XBeeCallbacks::transmitSuccess(uint8_t numAttempts,
                                    CTSNCommon::XBeeConstants::DiscoveryStatus discovery) {
    std::stringstream ss;
    ss << TRANSMIT_SUCCESS_MESSAGE << "Attempts: "
       << static_cast<unsigned short>(numAttempts) << "\n\tDiscovery: "
       << getDiscoveryString(discovery);
    m_outLogger.writeLineWithTimeStamp(ss.str());
}

void XBeeCallbacks::transmitFailure(uint8_t numAttempts,
                                    CTSNCommon::XBeeConstants::TxStatus errorNumber,
                                    CTSNCommon::XBeeConstants::DiscoveryStatus discovery) {
    std::stringstream ss;
    ss << TRANSMIT_FAILURE_MESSAGE << "Attempts: "
       << static_cast<unsigned short>(numAttempts) << "\n\tDiscovery: "
       << getDiscoveryString(discovery) << "\n\tReason: "
       << getTxFailureReason(errorNumber);
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

const std::string XBeeCallbacks::getDiscoveryString(CTSNCommon::XBeeConstants::DiscoveryStatus discovery) {
    std::string ret;
    switch(discovery) {
        case CTSNCommon::XBeeConstants::DiscoveryStatus::NO_OVERHEAD:
            ret = TRANSMIT_NO_DISCOVERY_OVERHEAD;
            break;
        case CTSNCommon::XBeeConstants::DiscoveryStatus::ROUTE_DISCOVERY:
            ret = TRANSMIT_ROUTE_DISCOVERY_NEEDED;
            break;
        default:
            ret = TRANSMIT_UNKNOWN_ROUTE_DISCOVERY;
    };
    return ret;
}

const std::string XBeeCallbacks::getTxFailureReason(CTSNCommon::XBeeConstants::TxStatus txStatus) {
    std::string ret;

    switch(txStatus) {
        case CTSNCommon::XBeeConstants::SUCCESS:
            ret = TX_STATUS_SUCCESS;
            break;
        case CTSNCommon::XBeeConstants::MAC_ACK_FAILURE:
            ret = TX_STATUS_MAC_ACK_FAILURE;
            break;
        case CTSNCommon::XBeeConstants::INVALID_ENDPOINT:
            ret = TX_STATUS_INVALID_ENDPOINT;
            break;
        case CTSNCommon::XBeeConstants::NETWORK_ACK_FAILURE:
            ret = TX_STATUS_NETWORK_ACK_FAILURE;
            break;
        case CTSNCommon::XBeeConstants::ROUTE_NOT_FOUND:
            ret = TX_STATUS_ROUTE_NOT_FOUND;
            break;
        default:
            ret = TX_STATUS_UNKNOWN;
            break;
    };

    return ret;
}

}

