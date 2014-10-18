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

