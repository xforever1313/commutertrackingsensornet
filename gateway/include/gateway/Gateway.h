#ifndef GATEWAY_H
#define GATEWAY_H

#include <istream>
#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/HTTPPosterInterface.h"
#include "EventExecutorInterface.h"
#include "gateway/MariaDBInterface.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/ShutdownInterface.h"
#include "ctsn_common/UartInterface.h"
#include "ctsn_common/UartRecvThread.h"
#include "ctsn_common/XBeeCallbackInterface.h"
#include "ctsn_common/XBeeController.h"
#include "io/LoggerBase.h"
#include "SMutex.h"
#include "SSemaphore.h"

namespace Gateway {

/**
 * \class Gateway
 * \brief Constructs and starts everything up.  It will
 *        then wait until a shutdown event occurs and it will
 *        tear everything down.
 */
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

        CTSNCommon::Settings &m_settings;
        Common::EventExecutorInterface *m_eventExecutor;
        std::istream *m_input;
        Common::IO::LoggerBase *m_output;

        CTSNCommon::UartInterface *m_uart;
        CTSNCommon::XBeeCallbackInterface *m_xbeeCallbacks;
        CTSNCommon::XBeeController *m_xbeeController;
        CTSNCommon::UartRecvThread *m_recvThread;

        OS::SMutex m_shutdownMutex;
        bool m_isShutdown;

        OS::SSemaphore m_shutdownSemaphore;

        Poco::Net::ServerSocket *m_socket;
        Poco::Net::HTTPServer *m_server;

        MariaDBInterface *m_mariadb;
        CTSNCommon::NodeContainerInterface *m_nodes;

        CTSNCommon::HTTPPosterInterface *m_httpPoster;
};

}

#endif // GATEWAY_H

