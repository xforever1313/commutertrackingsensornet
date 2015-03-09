#include <cstdint>
#include <stdexcept>
#include <mutex>
#include <vector>

#include "io/ConsoleLogger.h"
#include "ctsn_common/XBeeCallbackInterface.h"
#include "ctsn_common/XBeeConstants.h"
#include "ctsn_common/XBeeController.h"
#include "SThread.h"

namespace CTSNCommon {

// Include start char, length, address, and option bytes.
const uint8_t XBeeController::BYTES_TO_IGNORE = 15;
const uint8_t XBeeController::TX_BYTES_TO_IGNORE = 7;

XBeeController::XBeeController(XBeeCallbackInterface *callbacks) :
    OS::Runnable<XBeeController>(this),
    m_dataLength(0),  // Put in bad state until everything begins
    m_checkSumTotal(0),
    m_bytesProcessed(0),
    m_nonEscapedBytesProcessed(0),
    m_lengthCounter(0),
    m_isAlive(true),
    m_currentState(STARTUP),
    m_callbacks(callbacks),
    m_escapedCharacter(false),
    m_packetFrame(CTSNCommon::XBeeConstants::PacketFrame::UNKNOWN),
    m_modemStatus(CTSNCommon::XBeeConstants::ModemStatus::UNKNOWN_STATUS),
    m_transmitRetryCount(0),
    m_txStatus(CTSNCommon::XBeeConstants::TxStatus::UNKNOWN_TX_STATUS),
    m_discoveryStatus(CTSNCommon::XBeeConstants::DiscoveryStatus::UNKNOWN_DISCOVERY_STATUS)
{

}

XBeeController::~XBeeController() {
    kill();
    join(); // Wait for run loop to exit.

    // Handle the remaining pieces of data
    for(unsigned int i = 0; (i < 100) && !m_data.empty(); ++i) {
        handleData();
    }
}

void XBeeController::addData(const std::vector<std::uint8_t> &data) {
    for (size_t i = 0; i < data.size(); ++i) {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_data.push(data[i]);
        m_dataSemaphore.post();
    }
}

void XBeeController::kill(bool shutdownSemaphore /* = true */) {
    std::lock_guard<std::mutex> lock(m_isAliveMutex);
    m_isAlive = false;
    if (shutdownSemaphore) {
        m_dataSemaphore.shutdown();
    }
}

bool XBeeController::isAlive() {
    std::lock_guard<std::mutex> lock(m_isAliveMutex);
    return m_isAlive;
}

void XBeeController::run() {
    do {
        m_dataSemaphore.wait(); // Wait for data to be added on queue.

        // Ensure the wait wasn't because we shutdown.
        if (!m_dataSemaphore.isShutdown()) {
            try {
                handleData();
            }
            catch (const std::runtime_error &e) {
                Common::IO::ConsoleLogger::err.writeLineWithTimeStamp(e.what());
            }
        }
    }while (isAlive());
}

void XBeeController::handleData() {
    switch(m_currentState) {
        case STARTUP:
            handleStartupState();
            break;
        case MSG_START:
            handleMessageStartState();
            break;
        case GOT_LENGTH1:
            handleGotLength1State();
            break;
        case GOT_LENGTH2:
            handleGotLength2State();
            break;
        case IGNORE_OPTIONS:
            handleIgnoreOptionsState();
            break;
        case IGNORE_TX_STATUS_OPTIONS:
            handleIgnoreTxStatusOptions();
            break;
        case PARSE_PAYLOAD:
            handleParsePayloadState();
            break;
        case TX_PARSE_TX_RETRY:
            handleTxParseTxRetryState();
            break;
        case TX_PARSE_STATUS:
            handleTxParseStatusState();
            break;
        case TX_PARSE_DISCOVERY:
            handleTxParseDiscoveryState();
            break;
        case CHECK_CHECKSUM:
            handleCheckCheckSumState();
            break;
        case PARSE_MODEM_STATUS:
            handleParseModemStatusState();
            break;
        default: // Bad State
            handleBadState();
            break;
    };
}

void XBeeController::handleStartupState() {
    std::uint8_t data = getNextByte();
    if (data == CTSNCommon::XBeeConstants::START_CHARACTER) {
        m_currentState = MSG_START;
        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
    }
}

void XBeeController::handleMessageStartState() {
    std::uint8_t data = getNextByte();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == CTSNCommon::XBeeConstants::START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;

        // Do not increment m_lengthCounter since we are not past the length states yet.

        //Get the MSB of the length
        std::uint16_t expandedData = data << 8;
        m_dataLength |= expandedData;
        m_currentState = GOT_LENGTH1;
    }
}

void XBeeController::handleGotLength1State() {
    std::uint8_t data = getNextByte();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == CTSNCommon::XBeeConstants::START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;

        // Do not increment m_lengthCounter since we are not past the length states yet.

        //Get the LSB of the length
        m_dataLength |= data;
        m_currentState = GOT_LENGTH2;
    }
}

void XBeeController::handleGotLength2State() {
    std::uint8_t data = getNextByte();

    // This is the data frame.  This can only be,
    // on the RX end of things,
    // 0x88, 0x8A, 0x8B, 0x90, 0x91, 0x95, 0x97.
    // If anything else, this is an invalid message and
    // return to the start state. An escape character is
    // invalid here, so no need to check for that.
    if (data == CTSNCommon::XBeeConstants::START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        ++m_nonEscapedBytesProcessed;
        m_bytesProcessed.push_back(data);
        m_checkSumTotal += data;
        ++m_lengthCounter;
        if (data == CTSNCommon::XBeeConstants::PacketFrame::RECEIVE_PACKET){
            m_currentState = IGNORE_OPTIONS;
            m_packetFrame = static_cast<CTSNCommon::XBeeConstants::PacketFrame>(data);
        }
        else if (data == CTSNCommon::XBeeConstants::PacketFrame::MODEM_STATUS) {
            m_currentState = PARSE_MODEM_STATUS;
            m_packetFrame = static_cast<CTSNCommon::XBeeConstants::PacketFrame>(data);
        }
        else if (data == CTSNCommon::XBeeConstants::PacketFrame::TRANSMIT_STATUS) {
            // Ensure the length is 7, otherwise the packet is invalid
            if (m_dataLength != 0x07) {
                m_callbacks->badTxStatusPacket(m_bytesProcessed);
                reset();
            }
            else {
                m_currentState = IGNORE_TX_STATUS_OPTIONS;
                m_packetFrame = static_cast<CTSNCommon::XBeeConstants::PacketFrame>(data);
            }
        }
        else {
            m_callbacks->invalidPacketFrame(data);
            reset();
        }
    }
}

void XBeeController::handleParseModemStatusState() {
    std::uint8_t data = getNextByte();
    m_bytesProcessed.push_back(data);
    ++m_nonEscapedBytesProcessed;
    ++m_lengthCounter;

    m_checkSumTotal += data;

    // There should only be two bytes in a modem status frame:
    // the frame type and the status.  If the length is bigger than two,
    // Something bad occurred.  Return to the start state.
    if (m_dataLength != 2) {
         m_callbacks->badModemStatusPacket(m_bytesProcessed);
         reset();
    }
    else {
        m_modemStatus = static_cast<CTSNCommon::XBeeConstants::ModemStatus>(data);
        m_currentState = CHECK_CHECKSUM;
    }
}

void XBeeController::handleIgnoreOptionsState() {
    std::uint8_t data = getNextByte();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);

        // Escaped characters do not get added to the checksum
        // Nor get added to the length
    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == CTSNCommon::XBeeConstants::START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
        ++m_lengthCounter;

        m_checkSumTotal += data;
        if (m_nonEscapedBytesProcessed == BYTES_TO_IGNORE) {
            m_currentState = PARSE_PAYLOAD;
        }
    }
}

void XBeeController::handleIgnoreTxStatusOptions() {
    std::uint8_t data = getNextByte();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);

        // Escaped characters do not get added to the checksum
        // Nor get added to the length
    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == CTSNCommon::XBeeConstants::START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
        ++m_lengthCounter;

        m_checkSumTotal += data;
        if (m_nonEscapedBytesProcessed == TX_BYTES_TO_IGNORE) {
            m_currentState = TX_PARSE_TX_RETRY;
        }
    }

}

void XBeeController::handleParsePayloadState() {
    std::uint8_t data = getNextByte();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
        // Escaped characters do not get added to the checksum
        // Nor do not get counted in the length count

    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == CTSNCommon::XBeeConstants::START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
        ++m_lengthCounter;

        m_checkSumTotal += data;

        // data length + 3, as we need to add the start character,
        // and the two length characters.
        if (m_lengthCounter == static_cast<size_t>(m_dataLength)) {
            m_currentState = CHECK_CHECKSUM;
        }

        m_payload += static_cast<char>(data);
    }
}

void XBeeController::handleTxParseTxRetryState() {
   std::uint8_t data = getNextByte();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
        // Escaped characters do not get added to the checksum
        // Nor do not get counted in the length count

    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == CTSNCommon::XBeeConstants::START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
        ++m_lengthCounter;
        m_checkSumTotal += data;

        m_transmitRetryCount = data;

        m_currentState = TX_PARSE_STATUS;
    }
}

void XBeeController::handleTxParseStatusState() {
    std::uint8_t data = getNextByte();

    if (data == CTSNCommon::XBeeConstants::START_CHARACTER) {
         handleIncompleteMessage();
    }
    else {
        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
        ++m_lengthCounter;
        m_checkSumTotal += data;

        m_txStatus = static_cast<CTSNCommon::XBeeConstants::TxStatus>(data);

        m_currentState = TX_PARSE_DISCOVERY;
    }
}

void XBeeController::handleTxParseDiscoveryState() {
    std::uint8_t data = getNextByte();

    if (data == CTSNCommon::XBeeConstants::START_CHARACTER) {
         handleIncompleteMessage();
    }
    else {
        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;
        ++m_lengthCounter;
        m_checkSumTotal += data;

        m_discoveryStatus = static_cast<CTSNCommon::XBeeConstants::DiscoveryStatus>(data);

        m_currentState = CHECK_CHECKSUM;
    }
}

void XBeeController::handleCheckCheckSumState() {
    std::uint8_t data = getNextByte();

    // If we got the escape character from the last state,
    // xor it so we get the right checksum.
    if (data == CTSNCommon::XBeeConstants::ESCAPE_CHARACTER) {
        data = getNextByte(); // Ditch the escaped character
        data = data ^ CTSNCommon::XBeeConstants::ESCAPE_XOR;
    }

    if ((0xff - m_checkSumTotal) == data) {
        handleSuccessfulParse();
    }
    else if (data == CTSNCommon::XBeeConstants::START_CHARACTER){
        // Premature start character
        handleIncompleteMessage();
        return; // Return early so we don't reset twice.
    }
    else {
        // Bad checksum
        m_bytesProcessed.push_back(data);
        m_callbacks->badChecksum(m_bytesProcessed);
    }

    reset();
}

void XBeeController::handleSuccessfulParse() {
    switch(m_packetFrame) {
        case CTSNCommon::XBeeConstants::PacketFrame::RECEIVE_PACKET:
            m_callbacks->successfulParse(m_payload);
            break;

        case CTSNCommon::XBeeConstants::PacketFrame::MODEM_STATUS:
            switch(m_modemStatus) {
                case CTSNCommon::XBeeConstants::ModemStatus::HARDWARE_RESET:
                    m_callbacks->hardwareReset();
                    break;
                case CTSNCommon::XBeeConstants::ModemStatus::WATCHDOG_TIMER_RESET:
                    m_callbacks->watchdogTimerReset();
                    break;
                case CTSNCommon::XBeeConstants::ModemStatus::NETWORK_WENT_TO_SLEEP:
                    m_callbacks->networkWentToSleep();
                    break;
                case CTSNCommon::XBeeConstants::ModemStatus::NETWORK_WOKE_UP:
                    m_callbacks->networkWokeUp();
                    break;
                default:
                    m_callbacks->invalidModemStatus(m_modemStatus);
                    break;
            };
            break;

        case CTSNCommon::XBeeConstants::TRANSMIT_STATUS:
            if (m_txStatus == CTSNCommon::XBeeConstants::TxStatus::SUCCESS) {
                m_callbacks->transmitSuccess(m_transmitRetryCount, m_discoveryStatus);
            }
            else {
                m_callbacks->transmitFailure(m_transmitRetryCount,
                                             m_txStatus,
                                             m_discoveryStatus);
            }
            break;

        default:
            // Technically, this shouldnt happen, but just in case.
            m_callbacks->badState(m_bytesProcessed);
            break;
    };
}

void XBeeController::handleIncompleteMessage() {
    m_callbacks->incompleteMessage(m_bytesProcessed);
    reset();
    m_bytesProcessed.push_back(CTSNCommon::XBeeConstants::START_CHARACTER);
    m_currentState = MSG_START;
}

void XBeeController::reset() {
    m_currentState = STARTUP;
    m_dataLength = 0;
    m_bytesProcessed.clear();
    m_checkSumTotal = 0;
    m_payload = "";
    m_escapedCharacter = false;
    m_nonEscapedBytesProcessed = 0;
    m_lengthCounter = 0;
    m_packetFrame = CTSNCommon::XBeeConstants::PacketFrame::UNKNOWN;
    m_modemStatus = CTSNCommon::XBeeConstants::ModemStatus::UNKNOWN_STATUS;
    m_transmitRetryCount = 0;
    m_txStatus = CTSNCommon::XBeeConstants::TxStatus::UNKNOWN_TX_STATUS;
    m_discoveryStatus = CTSNCommon::XBeeConstants::DiscoveryStatus::UNKNOWN_DISCOVERY_STATUS;
}

uint8_t XBeeController::getNextByte() {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    if (m_data.empty()) {
        throw std::runtime_error("Empty Queue!");
    }
    std::uint8_t data = m_data.front();
    m_data.pop();
    return data;
}

void XBeeController::handleBadState() {
    m_callbacks->badState(m_bytesProcessed);
    reset();
}

}

