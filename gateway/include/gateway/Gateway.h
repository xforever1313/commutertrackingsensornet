#ifndef GATEWAY_H
#define GATEWAY_H

#include <istream>
#include <Poco/Net/HTTPServer.h>

#include "EventExecutorInterface.h"
#include "gateway/ShutdownInterface.h"
#include "gateway/UartInterface.h"
#include "gateway/UartRecvThread.h"
#include "io/LoggerBase.h"
#include "SMutex.h"

namespace Gateway {

class Gateway : public ShutdownInterface {
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
        void shutdown() override;
        bool isShutdown();

        Common::EventExecutorInterface *m_eventExecutor;
        std::istream *m_input;
        Common::IO::LoggerBase *m_output;

        UartInterface *m_uart;
        UartRecvThread *m_recvThread;

        OS::SMutex m_shutdownMutex;
        bool m_isShutdown;

        Poco::Net::HTTPServer *m_server;
};

}

#endif // GATEWAY_H
