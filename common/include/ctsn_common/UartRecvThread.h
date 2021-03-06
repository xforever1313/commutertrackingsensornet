#ifndef UARTRECVTHREAD_H
#define UARTRECVTHREAD_H

#include <mutex>

#include "ctsn_common/UartInterface.h"
#include "ctsn_common/UartRecvCallbackInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace CTSNCommon {

/**
 * \class UartRecvThread
 * \brief This class has one job, read stuff from uart,
 *        and call the callback with the data and go back to reading.
 */
class UartRecvThread : public OS::Runnable<UartRecvThread> {
    public:
        /**
         * \note Constructor only constructs object, start() must be called before the thread begins.
         */
        UartRecvThread(CTSNCommon::UartInterface *uart,
                       UartRecvCallbackInterface *callback,
                       Common::IO::LoggerBase &errorLogger = Common::IO::ConsoleLogger::err);

        /**
         * \brief Destructor - kills and joins the thread.
         * \warning Ensure the Uart is CLOSED before destruction, or a deadlock will occur.
         */
        ~UartRecvThread();

        /**
         * \brief Call when there is data ready.  Usually called by an interrupt.
         */
        void dataReady();

        void kill();

        bool isAlive();

        void run();

    private:
        UartRecvThread() = delete;

        CTSNCommon::UartInterface *m_uart;
        UartRecvCallbackInterface *m_callback;
        Common::IO::LoggerBase &m_errorLogger;
        OS::SConditionVariable m_dataCV;
        bool m_isAlive;
        std::mutex m_isAliveMutex;
};

}

#endif // UARTRECVTHREAD_H
