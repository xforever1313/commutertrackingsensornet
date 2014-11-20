#ifndef GATEWAY_H
#define GATEWAY_H

#include <istream>
#include <Poco/Net/HTTPServer.h>

#include "EventExecutorInterface.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeContainerInterface.h"
#include "ctsn_common/ShutdownInterface.h"
#include "gateway/UartInterface.h"
#include "gateway/UartRecvThread.h"
#include "gateway/XBeeCallbackInterface.h"
#include "gateway/XBeeController.h"
#include "io/LoggerBase.h"
#include "SMutex.h"
#include "SSemaphore.h"

namespace Gateway {

class Gateway : public CTSNCommon::ShutdownInterface {
    public:
        static Gateway &getInstance();
        virtual ~Gateway();

        void start();
    private:
        static void RxSignal(int status);

        Gateway();
        Gateway(const Gateway &other) = delete;

        void initHTTPServer();
        void initMariaDB();
        void shutdown() override;

        Common::EventExecutorInterface *m_eventExecutor;
        std::istream *m_input;
        Common::IO::LoggerBase *m_output;

        UartInterface *m_uart;
        XBeeCallbackInterface *m_xbeeCallbacks;
        XBeeController *m_xbeeController;
        UartRecvThread *m_recvThread;

        OS::SMutex m_shutdownMutex;
        bool m_isShutdown;

        OS::SSemaphore m_shutdownSemaphore;

        Poco::Net::ServerSocket *m_socket;
        Poco::Net::HTTPServer *m_server;

        MariaDBInterface *m_mariadb;
        NodeContainerInterface *m_nodes;
};

}

#endif // GATEWAY_H

