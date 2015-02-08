#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/PiGPIOController.h"
#include "ctsn_common/Uart.h"
#include "ctsn_common/UartRecvThread.h"
#include "ctsn_common/XBeeCallbacks.h"
#include "ctsn_common/XBeeController.h"
#include "CTSNSharedGlobals.py"
#include "EventExecutor.h"
#include "io/ConsoleLogger.h"
#include "pi_node/NodeContainer.h"
#include "pi_node/PinNumbers.h"
#include "pi_node/HTTPRequestFactory.h"
#include "pi_node/PiNode.h"
#include "pi_node/StatusLed.h"

namespace PiNode  {

void PiNode::RxSignal(int status) {
    getInstance().m_xbeeController->yield();
    getInstance().m_recvThread->dataReady();
}

PiNode &PiNode::getInstance() {
    static PiNode piNode;
    return piNode;
}

PiNode::PiNode() :
    m_settings(CTSNCommon::Settings::getInstance()),
    m_eventExecutor(new Common::EventExecutor()),
    m_socket(nullptr),
    m_server(nullptr),
    m_uart(new CTSNCommon::Uart(&RxSignal)),
    m_xbeeCallbacks(new CTSNCommon::XBeeCallbacks(m_settings.getShortSetting("NODE_PORT"))),
    m_xbeeController(new CTSNCommon::XBeeController(m_xbeeCallbacks)),
    m_recvThread(new CTSNCommon::UartRecvThread(m_uart, m_xbeeController)),
    m_gpio(CTSNCommon::PiGPIOController::getInstance()),
    m_statusLed(new StatusLed(PinNumbers::STATUS_LED, 500, m_gpio)),
    m_nodes(new NodeContainer())
{

}

PiNode::~PiNode() {
    delete m_server;
    delete m_socket;
    delete m_eventExecutor;
    delete m_recvThread;
    delete m_xbeeController;
    delete m_xbeeCallbacks;
    delete m_uart;
    delete m_statusLed;
    delete m_nodes;
}

void PiNode::initHTTPServer() {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(100);
    params->setMaxThreads(1);

    m_socket = new Poco::Net::ServerSocket(m_settings.getShortSetting("NODE_PORT"));
    m_server = new Poco::Net::HTTPServer(new HTTPRequestFactory(this,
                                                                m_gpio,
                                                                m_nodes,
                                                                m_eventExecutor,
                                                                m_uart),
                                         *m_socket, params);
}

void  PiNode::start() {
    bool serverStarted = false;
    try {
        m_nodes->refreshNodes();
        initHTTPServer();
        m_server->start();
        serverStarted = true;

        m_uart->open(m_settings.getSetting("NODE_SERIAL"));
        m_xbeeController->start();
        m_recvThread->start();
        m_statusLed->start();
    }
    catch (const std::exception &e) {
        Common::IO::ConsoleLogger::err.writeLineWithTimeStamp(e.what());
    }
    m_shutdownCV.wait();

    if (serverStarted) {
        m_server->stop();
    }

    m_recvThread->kill();
    m_xbeeController->kill();
    m_uart->close();
    m_statusLed->kill();
}

void PiNode::shutdown() {
    m_shutdownCV.shutdown();
}

}

