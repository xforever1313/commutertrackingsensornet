#ifndef PI_NODE_H_
#define PI_NODE_H_

#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/ShutdownInterface.h"
#include "EventExecutorInterface.h"

#include "SSemaphore.h"

namespace PiNode {

class PiNode : public CTSNCommon::ShutdownInterface {
    public:
        static PiNode &getInstance();

        ~PiNode();

        void initHTTPServer();

        void start();

        void shutdown() override;

    private:
        PiNode();
        Common::EventExecutorInterface *m_eventExecutor;
        OS::SSemaphore m_shutdownSemaphore;

        Poco::Net::ServerSocket *m_socket;
        Poco::Net::HTTPServer *m_server;
};

}

#endif

