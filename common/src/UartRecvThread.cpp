#include <mutex>
#include <stdexcept>

#ifdef LINUX
#include <sys/signal.h>     //Used for interrupt
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>         //Used for UART
#endif

#include "ctsn_common/UartInterface.h"
#include "ctsn_common/UartRecvCallbackInterface.h"
#include "ctsn_common/UartRecvThread.h"
#include "io/LoggerBase.h"
#include "SSemaphore.h"

namespace CTSNCommon {

UartRecvThread::UartRecvThread(CTSNCommon::UartInterface *uart,
                               UartRecvCallbackInterface *callback,
                               Common::IO::LoggerBase &errorLogger /*= Common::IO::ConsoleLogger::err*/) :
    OS::Runnable<UartRecvThread>(this),
    m_uart(uart),
    m_callback(callback),
    m_errorLogger(errorLogger),
    m_isAlive(true)
{
}

UartRecvThread::~UartRecvThread() {
    kill(); //Force thread to exit.
    m_dataCV.shutdown();
    join();
}

void UartRecvThread::dataReady() {
    m_dataCV.notifyOne();
}

void UartRecvThread::kill() {
    std::lock_guard<std::mutex> lock(m_isAliveMutex);
    m_isAlive = false;
}

bool UartRecvThread::isAlive() {
    std::lock_guard<std::mutex> lock(m_isAliveMutex);
    return m_isAlive;
}

void UartRecvThread::run() {
    do {
        try {
            m_dataCV.wait();
            // Only look for data if the semaphore is not shut down.
            if (!m_dataCV.isShutdown()) {
                std::vector<std::uint8_t> data = m_uart->recvBinary();
                m_callback->addData(data);
            }
        }
        catch(const std::runtime_error &e) {
            m_errorLogger.writeLineWithTimeStamp(e.what());
        }

    } while(isAlive());
}

}

