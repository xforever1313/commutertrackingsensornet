#include <cstdint>
#include <vector>

#include "gateway/XBeeCallbackInterface.h"
#include "gateway/XBeeController.h"

namespace Gateway {

const uint8_t XBeeController::START_CHARACTER = '~';

// Ignore start char, length, address, and option bytes.
const uint8_t XBeeController::BYTES_TO_IGNORE = 17;

XBeeController::XBeeController(XBeeCallbackInterface *callbacks) :
    m_dataLength(0),  // Put in bad state until everything begins
    m_checkSumTotal(0),
    m_bytesProcessed(0),
    m_isAlive(true),
    m_currentState(STARTUP),
    m_callbacks(callbacks)
{

}

XBeeController::~XBeeController() {
    kill();
    m_dataSemaphore.shutdown();
    join(); // Wait for run loop to exit.

    // Handle the remaining pieces of data
    //while (!m_data.empty()) {
    //    handleData();
    //}
}

void XBeeController::addData(const std::vector<std::uint8_t> &data) {
    for (size_t i = 0; i < data.size(); ++i) {
        m_data.push(data[i]);
        m_dataSemaphore.post();
    }
}

void XBeeController::kill() {
    m_isAliveMutex.lock();
    m_isAlive = false;
    m_isAliveMutex.unlock();
}

bool XBeeController::isAlive() {
    bool ret;
    m_isAliveMutex.lock();
    ret = m_isAlive;
    m_isAliveMutex.unlock();

    return ret;
}

void XBeeController::run() {
    do {
        m_dataSemaphore.wait(); // Wait for data to be added on queue.

        // Ensure the wait wasn't because we shutdown.
        if (!m_dataSemaphore.isShutdown()) {
            handleData();
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
        case PARSE_PAYLOAD:
            handleParsePayloadState();
            break;
        case CHECK_CHECKSUM:
            handleCheckCheckSumState();
            break;
        case INCOMPLETE_MESSAGE:
            break;
        default: // Bad State
            break;
    };
}

void XBeeController::handleStartupState() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    if (data == START_CHARACTER) {
        m_currentState = MSG_START;
        m_bytesProcessed.push_back(data);
    }
}

void XBeeController::handleMessageStartState() {
    std::uint16_t data = m_data.front();
    m_data.pop();
    m_bytesProcessed.push_back(data);
    /// \todo insert incomplete state

    //Get the MSB of the length
    data = data << 8; 
    m_dataLength |= data;        
    m_currentState = GOT_LENGTH1;
}

void XBeeController::handleGotLength1State() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    m_bytesProcessed.push_back(data);

    /// \todo insert incomplete state

    //Get the LSB of the length
    m_dataLength |= data;
    m_currentState = GOT_LENGTH2;
}

void XBeeController::handleGotLength2State() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    m_bytesProcessed.push_back(data);

    /// \todo insert incomplete state

    m_checkSumTotal += data;
    m_currentState = IGNORE_OPTIONS;
}

void XBeeController::handleIgnoreOptionsState() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    m_bytesProcessed.push_back(data);

    /// \todo insert incomplete state
    m_checkSumTotal += data;
    if (m_bytesProcessed.size() == BYTES_TO_IGNORE) {
        m_currentState = PARSE_PAYLOAD;
    }
}

void XBeeController::handleParsePayloadState() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    m_bytesProcessed.push_back(data);

    /// \todo insert incomplete state
    m_checkSumTotal += data;

    // data length + 3, as we need to add the start character,
    // and the two length characters.
    if (m_bytesProcessed.size() == static_cast<size_t>(m_dataLength + 3)) {
        m_currentState = CHECK_CHECKSUM;
    }

    m_payload += static_cast<char>(data);
}

void XBeeController::handleCheckCheckSumState() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    m_bytesProcessed.push_back(data);

    /// \todo check for incomplete state AFTER we check
    ///       the checksum.

    if ((0xff - m_checkSumTotal) == data) {
        //good checksum
    }
    else {
        //bad checksum.  If ~, new message.
    }

    reset();
}

void XBeeController::reset() {
    m_currentState = STARTUP;
    m_dataLength = 0;
    m_bytesProcessed.clear();
    m_checkSumTotal = 0;
}

}

