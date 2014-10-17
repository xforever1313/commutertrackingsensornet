#include <cstdint>
#include <vector>

#include "gateway/XBeeController.h"

namespace Gateway {

const uint8_t XBeeController::START_CHARACTER = '~';
const uint8_t XBeeController::BYTES_TO_IGNORE = 14;

XBeeController::XBeeController() :
    m_dataLength(0),  // Put in bad state until everything begins
    m_checkSumTotal(0),
    m_bytesProcessed(0),
    m_isAlive(true),
    m_currentState(STARTUP)
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
    }
}

void XBeeController::handleMessageStartState() {
    std::uint16_t data = m_data.front();
    m_data.pop();
    /// \todo insert incomplete state

    //Get the MSB of the length
    data = data << 8; 
    m_dataLength |= data;        
    m_currentState = GOT_LENGTH1;
}

void XBeeController::handleGotLength1State() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    /// \todo insert incomplete state

    //Get the LSB of the length
    m_dataLength |= data;
    m_currentState = GOT_LENGTH2;
}

void XBeeController::handleGotLength2State() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    /// \todo insert incomplete state

    m_checkSumTotal += data;
    m_currentState = IGNORE_OPTIONS;
    ++m_bytesProcessed;
}

void XBeeController::handleIgnoreOptionsState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    /// \todo insert incomplete state
    m_checkSumTotal += data;
    if (++m_bytesProcessed == BYTES_TO_IGNORE) {
        m_currentState = PARSE_PAYLOAD;
    }
}

void XBeeController::handleParsePayloadState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    /// \todo insert incomplete state
    m_checkSumTotal += data;
    if (++m_bytesProcessed == (m_dataLength)) {
        m_currentState = CHECK_CHECKSUM;
    }

    m_payload += static_cast<char>(data);
}

void XBeeController::handleCheckCheckSumState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

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
    m_bytesProcessed = 0;
    m_checkSumTotal = 0;
}

}

