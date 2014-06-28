#ifndef GATEWAY_H
#define GATEWAY_H

#include <istream>

#include "EventExecutorInterface.h"
#include "gateway/UartInterface.h"
#include "gateway/UartRecvThread.h"
#include "io/LoggerBase.h"

namespace Gateway {

class Gateway {
    public:
        static Gateway &getInstance();
        virtual ~Gateway();

        void start();
    private:
        static void RxSignal(int status);

        Gateway();
        Gateway(const Gateway &other) = delete;
        
        void sendEmail();
        void sendTextMessage();

        Common::EventExecutorInterface *m_eventExecutor;
        std::istream *m_input;
        Common::IO::LoggerBase *m_output;

        UartInterface *m_uart;
        UartRecvThread *m_recvThread;
};

}

#endif // GATEWAY_H
