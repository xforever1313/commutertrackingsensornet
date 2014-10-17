#include <cstdint>
#include <vector>

#include "gateway/XBeeController.h"

namespace Gateway {

const uint8_t XBeeController::START_CHARACTER = '~';

XBeeController::XBeeController() :
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
            break;
        case GOT_LENGTH1:
            break;
        case GOT_LENGTH2:
            break;
        case IGNORE_OPTIONS:
            break;
        case PARSE_MESSAGE:
            break;
        case CHECK_CHECKSUM:
            break;
        case SEND_COMMAND:
            break;
        case BAD_CHECKSUM:
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

}

