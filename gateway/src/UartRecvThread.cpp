#include <stdexcept>

#ifdef LINUX
#include <sys/signal.h>     //Used for interrupt
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>         //Used for UART
#endif

#include "ctsn_common/UartInterface.h"
#include "gateway/UartRecvCallbackInterface.h"
#include "gateway/UartRecvThread.h"
#include "io/LoggerBase.h"
#include "SSemaphore.h"

namespace Gateway {

#ifdef LINUX
/**
 * \brief sends sigio to the uart so it returns upon destruction.  This MUST be created 
 *        in the run loop.
 * \todo If using windows, a new version of this must be created since this
 *       is strictly linux specific
 */
struct UartRecvThread::UartRecvImpl {
    UartRecvImpl() :
        // Get the thread id.
        m_tid(syscall(SYS_gettid))
    {
    }

    ~UartRecvImpl() {
        #ifndef UNIT_TEST
        // Send a sigio to the thread so uart read() returns.
        ::kill(m_tid, SIGIO);
        #endif
    }

    const pid_t m_tid;
};

#else
#error Platform not supported, implement a UartRecvImpl.
#endif

UartRecvThread::UartRecvThread(CTSNCommon::UartInterface *uart,
                               UartRecvCallbackInterface *callback,
                               Common::IO::LoggerBase &errorLogger /*= Common::IO::ConsoleLogger::err*/) :
    m_uart(uart),
    m_callback(callback),
    m_errorLogger(errorLogger),
    m_isAlive(true),
    m_impl(nullptr)
{
}

UartRecvThread::~UartRecvThread() {
    kill(); //Force thread to exit.
    m_dataSemaphore.shutdown();
    delete m_impl;
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
    m_impl = new UartRecvImpl();
    do {
        try {
            m_dataSemaphore.wait();
            // Only look for data if the semaphore is not shut down.
            if (!m_dataSemaphore.isShutdown()) {
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

