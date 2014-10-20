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

void XBeeController::kill(bool shutdownSemaphore /* = true */) {
    m_isAliveMutex.lock();
    m_isAlive = false;
    m_isAliveMutex.unlock();
    if (shutdownSemaphore) {
        m_dataSemaphore.shutdown();
    }
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
        default: // Bad State
            handleBadState();
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

    if (data == START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        m_bytesProcessed.push_back(data);

        //Get the MSB of the length
        data = data << 8; 
        m_dataLength |= data;        
        m_currentState = GOT_LENGTH1;
    }
}

void XBeeController::handleGotLength1State() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    if (data == START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        //Get the LSB of the length
        m_bytesProcessed.push_back(data);
        m_dataLength |= data;
        m_currentState = GOT_LENGTH2;
    }
}

void XBeeController::handleGotLength2State() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    if (data == START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        m_bytesProcessed.push_back(data);
        m_checkSumTotal += data;
        m_currentState = IGNORE_OPTIONS;
    }
}

void XBeeController::handleIgnoreOptionsState() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    if (data == START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        m_bytesProcessed.push_back(data);
        m_checkSumTotal += data;
        if (m_bytesProcessed.size() == BYTES_TO_IGNORE) {
            m_currentState = PARSE_PAYLOAD;
        }
    }
}

void XBeeController::handleParsePayloadState() {
    std::uint8_t data = m_data.front();
    m_data.pop();
    if (data == START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        m_bytesProcessed.push_back(data);

        m_checkSumTotal += data;

        // data length + 3, as we need to add the start character,
        // and the two length characters.
        if (m_bytesProcessed.size() == static_cast<size_t>(m_dataLength + 3)) {
            m_currentState = CHECK_CHECKSUM;
        }

        m_payload += static_cast<char>(data);
    }
}

void XBeeController::handleCheckCheckSumState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    if ((0xff - m_checkSumTotal) == data) {
        m_callbacks->successfulParse(m_payload);
    }
    else if (data == START_CHARACTER){
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

void XBeeController::handleIncompleteMessage() {
    m_callbacks->incompleteMessage(m_bytesProcessed);
    reset();
    m_bytesProcessed.push_back(START_CHARACTER);
    m_currentState = MSG_START;
}

void XBeeController::reset() {
    m_currentState = STARTUP;
    m_dataLength = 0;
    m_bytesProcessed.clear();
    m_checkSumTotal = 0;
    m_payload = "";
}

void XBeeController::handleBadState() {
    m_callbacks->badState(m_bytesProcessed);
    reset();
}

}

