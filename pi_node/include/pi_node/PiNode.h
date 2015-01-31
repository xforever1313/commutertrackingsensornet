#ifndef PI_NODE_H_
#define PI_NODE_H_

#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/PiGPIOController.h"
#include "ctsn_common/ShutdownInterface.h"
#include "ctsn_common/UartInterface.h"
#include "ctsn_common/UartRecvThread.h"
#include "ctsn_common/XBeeCallbackInterface.h"
#include "ctsn_common/XBeeController.h"
#include "EventExecutorInterface.h"
#include "pi_node/StatusLed.h"
#include "SConditionVariable.h"

namespace PiNode {

class PiNode : public CTSNCommon::ShutdownInterface {
    public:
        static PiNode &getInstance();

        ~PiNode();

        void initHTTPServer();

        void start();

        void shutdown() override;

    private:
        static void RxSignal(int status);

        PiNode();
        Common::EventExecutorInterface *m_eventExecutor;
        OS::SConditionVariable m_shutdownCV;

        Poco::Net::ServerSocket *m_socket;
        Poco::Net::HTTPServer *m_server;
        CTSNCommon::UartInterface *m_uart;
        CTSNCommon::XBeeCallbackInterface *m_xbeeCallbacks;
        CTSNCommon::XBeeController *m_xbeeController;
        CTSNCommon::UartRecvThread *m_recvThread;
        CTSNCommon::PiGPIOController &m_gpio;
        StatusLed *m_statusLed;
};

}

#endif

