#include <stdexcept>

#include "gateway/UartInterface.h"
#include "gateway/UartRecvThread.h"
#include "io/LoggerBase.h"
#include "SSemaphore.h"

namespace Gateway {

const std::string UartRecvThread::MESSAGE_PREFIX = "UART RX>";

UartRecvThread::UartRecvThread(UartInterface *uart,
                               Common::IO::LoggerBase &outLogger /*= Common::IO::ConsoleLogger::out*/,
                               Common::IO::LoggerBase &errorLogger /*= Common::IO::ConsoleLogger::err*/) :
    m_uart(uart),
    m_outLogger(outLogger),
    m_errorLogger(errorLogger),
    m_isAlive(true)
{
}

UartRecvThread::~UartRecvThread() {
    kill(); //Force thread to exit.
    m_dataSemaphore.shutdown();
    join();
}

void UartRecvThread::dataReady() {
    m_dataSemaphore.post();
}

void UartRecvThread::kill() {
    m_isAliveMutex.lock();
    m_isAlive = false;
    m_isAliveMutex.unlock();
}

bool UartRecvThread::isAlive() {
    bool ret;
    m_isAliveMutex.lock();
    ret = m_isAlive;
    m_isAliveMutex.unlock();

    return ret;
}

void UartRecvThread::run() {
    do {
        m_dataSemaphore.wait();
        std::string message;
        try {
            std::string subMessage;
            do {
                subMessage = m_uart->recv();
                message += subMessage;
            } while (subMessage != "");

            m_outLogger.writeLine(MESSAGE_PREFIX + message);
        }
        catch(const std::runtime_error &e) {
            m_errorLogger.writeLine(MESSAGE_PREFIX + e.what());
        }

    } while(isAlive());
}

}
