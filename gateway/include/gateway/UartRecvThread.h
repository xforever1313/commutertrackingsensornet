#ifndef UARTRECVTHREAD_H
#define UARTRECVTHREAD_H

#include "gateway/UartInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"
#include "SMutex.h"
#include "SSemaphore.h"
#include "SThread.h"

namespace Gateway {

class UartRecvThread : public OS::SThread {
    public:
        /**
         * \note Constructor only constructs object, start() must be called before the thread begins.
         */
        UartRecvThread(UartInterface *uart,
                       Common::IO::LoggerBase &outLogger = Common::IO::ConsoleLogger::out,
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

    private:
        static const std::string MESSAGE_PREFIX;

        UartRecvThread() = delete;
        void run() override;

        UartInterface *m_uart;
        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errorLogger;
        OS::SSemaphore m_dataSemaphore;
        bool m_isAlive;
        OS::SMutex m_isAliveMutex;
};

}

#endif // UARTRECVTHREAD_H
