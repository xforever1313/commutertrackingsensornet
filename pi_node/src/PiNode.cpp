#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/Uart.h"
#include "CTSNSharedGlobals.py"
#include "EventExecutor.h"
#include "io/ConsoleLogger.h"
#include "pi_node/HTTPRequestFactory.h"
#include "pi_node/PiNode.h"

namespace PiNode  {

void PiNode::RxSignal(int status) {

}

PiNode &PiNode::getInstance() {
    static PiNode piNode;
    return piNode;
}

PiNode::PiNode() :
    m_eventExecutor(new Common::EventExecutor()),
    m_socket(nullptr),
    m_server(nullptr),
    m_uart(new CTSNCommon::Uart(&RxSignal))
{

}

PiNode::~PiNode() {
    delete m_server;
    delete m_socket;
    delete m_eventExecutor;
    delete m_uart;
}

void PiNode::initHTTPServer() {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(100);
    params->setMaxThreads(1);

    m_socket = new Poco::Net::ServerSocket(PI_NODE_COMMAND_PORT);
    m_server = new Poco::Net::HTTPServer(new HTTPRequestFactory(this,
                                                                m_eventExecutor),
                                         *m_socket, params);
}

void  PiNode::start() {
    bool serverStarted = true;
    try {
        initHTTPServer();
        m_server->start();
    }
    catch (const std::exception &e) {
        Common::IO::ConsoleLogger::err.writeLineWithTimeStamp(e.what());
    }
    m_shutdownSemaphore.wait();

    if (serverStarted) {
        m_server->stop();
    }
}

void PiNode::shutdown() {
    m_shutdownSemaphore.shutdown();
}

}

