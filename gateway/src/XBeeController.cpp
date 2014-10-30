#include <cstdint>
#include <vector>

#include "gateway/XBeeCallbackInterface.h"
#include "gateway/XBeeController.h"

namespace Gateway {

const uint8_t XBeeController::START_CHARACTER = '~';
const uint8_t XBeeController::ESCAPE_CHARACTER = 0x7D;
const uint8_t XBeeController::XON = 0x11;
const uint8_t XBeeController::XOFF = 0x13;
const uint8_t XBeeController::ESCAPE_XOR = 0x20;

// Include start char, length, address, and option bytes.
const uint8_t XBeeController::BYTES_TO_IGNORE = 15;

XBeeController::XBeeController(XBeeCallbackInterface *callbacks) :
    m_dataLength(0),  // Put in bad state until everything begins
    m_checkSumTotal(0),
    m_bytesProcessed(0),
    m_nonEscapedBytesProcessed(0),
    m_lengthCounter(0),
    m_isAlive(true),
    m_currentState(STARTUP),
    m_callbacks(callbacks),
    m_escapedCharacter(false)
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
        ++m_nonEscapedBytesProcessed;
    }
}

void XBeeController::handleMessageStartState() {
    std::uint16_t data = m_data.front();
    m_data.pop();
    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
    }
    // If we get a start character that is not escaped, 
    // we got an incomplete message
    else if ((data == START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ ESCAPE_XOR;
        }

        m_bytesProcessed.push_back(data);
        ++m_nonEscapedBytesProcessed;

        // Do not increment m_lengthCounter since we are not past the length states yet.

        //Get the MSB of the length
        data = data << 8;
        m_dataLength |= data;
        m_currentState = GOT_LENGTH1;
    }
}

void XBeeController::handleGotLength1State() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ ESCAPE_XOR;
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
    std::uint8_t data = m_data.front();
    m_data.pop();

    // This is the data frame.  This can only be,
    // on the RX end of things,
    // 0x88, 0x8A, 0x8B, 0x90, 0x91, 0x95, 0x97.
    // If anything else, this is an invalid message and 
    // return to the start state. An escape character is
    // invalid here, so no need to check for that.
    // \todo actually do this.

    if (data == START_CHARACTER) {
        handleIncompleteMessage();
    }
    else {
        ++m_nonEscapedBytesProcessed;
        m_bytesProcessed.push_back(data);
        m_checkSumTotal += data;
        m_currentState = IGNORE_OPTIONS;
        ++m_lengthCounter;
    }
}

void XBeeController::handleIgnoreOptionsState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);

        // Escaped characters do not get added to the checksum
        // Nor get added to the length
    }
    // If we get a start character that is not escaped, 
    // we got an incomplete message
    else if ((data == START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) { 
            m_escapedCharacter = false;
            data = data ^ ESCAPE_XOR;
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

void XBeeController::handleParsePayloadState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    // If we are not escaped and we get an escaped character
    // set the escaped character flag true
    if ((data == ESCAPE_CHARACTER) && !m_escapedCharacter) {
        m_escapedCharacter = true;
        m_bytesProcessed.push_back(data);
        // Escaped characters do not get added to the checksum
        // Nor do not get counted in the length count

    }
    // If we get a start character that is not escaped,
    // we got an incomplete message
    else if ((data == START_CHARACTER) && !m_escapedCharacter) {
        handleIncompleteMessage();
    }
    else {
        // If we are escaped, xor by 0x02, the value by which
        // all escaped values are xored by.
        if (m_escapedCharacter) {
            m_escapedCharacter = false;
            data = data ^ ESCAPE_XOR;
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

void XBeeController::handleCheckCheckSumState() {
    std::uint8_t data = m_data.front();
    m_data.pop();

    // If we got the escape character from the last state,
    // xor it so we get the right checksum.
    if (data == ESCAPE_CHARACTER) {
        data = m_data.front(); // Ditch the escaped character
        m_data.pop();
        data = data ^ ESCAPE_XOR;
    }

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
    m_escapedCharacter = false;
    m_nonEscapedBytesProcessed = 0;
    m_lengthCounter = 0;
}

void XBeeController::handleBadState() {
    m_callbacks->badState(m_bytesProcessed);
    reset();
}

}

