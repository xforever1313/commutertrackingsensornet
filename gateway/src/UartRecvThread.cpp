#include <stdexcept>

#include "gateway/UartInterface.h"
#include "gateway/UartRecvCallbackInterface.h"
#include "gateway/UartRecvThread.h"
#include "io/LoggerBase.h"
#include "SSemaphore.h"

namespace Gateway {

UartRecvThread::UartRecvThread(UartInterface *uart,
                               UartRecvCallbackInterface *callback,
                               Common::IO::LoggerBase &errorLogger /*= Common::IO::ConsoleLogger::err*/) :
    m_uart(uart),
    m_callback(callback),
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
        try {
            m_dataSemaphore.wait();
            std::vector<std::uint8_t> data = m_uart->recvBinary();
            m_callback->addData(data);
        }
        catch(const std::runtime_error &e) {
            m_errorLogger.writeLineWithTimeStamp(e.what());
        }

    } while(isAlive());
}

}
