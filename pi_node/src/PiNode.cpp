#include <Poco/Net/HTTPServer.h>
#include <stdexcept>
#include <sys/signal.h>
#include <unistd.h>

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
    getInstance().m_recvThread->dataReady();
}

PiNode &PiNode::getInstance() {
    static PiNode piNode;
    return piNode;
}

PiNode::PiNode() :
    m_settings(CTSNCommon::Settings::getInstance()),
    m_eventExecutor(new Common::EventExecutor()),
    m_cvExecutor(new Common::EventExecutor()),
    m_socket(nullptr),
    m_server(nullptr),
    m_uart(new CTSNCommon::Uart(&RxSignal)),
    m_xbeeCallbacks(new CTSNCommon::XBeeCallbacks(m_settings.getShortSetting("NODE_PORT"), m_settings.getSetting("NODE_AGENT"))),
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
    delete m_cvExecutor;
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
                                                                m_cvExecutor,
                                                                m_uart),
                                         *m_socket, params);
}

void  PiNode::start() {
    bool serverStarted = false;
    try {
        m_cvExecutor->startExecutor();
        m_eventExecutor->startExecutor();

        m_nodes->refreshNodes();
        initHTTPServer();
        m_server->start();
        serverStarted = true;

        m_uart->open(m_settings.getSetting("NODE_SERIAL"));
        m_xbeeController->start();
        m_recvThread->start();
        m_statusLed->start();

        // Start the picture process
        pid_t pid = fork();

        // Child process
        if (pid == 0) {
            std::string userAgentArg = "--user_agent=" + m_settings.getSetting("NODE_AGENT");
            std::string uriArg = "--uri=" + PICTURE_PARSE_URI;
            std::string portArg = "--port=" + std::to_string(m_settings.getShortSetting("NODE_PORT"));
            std::string pictureOutputArg = "--pic_output=" + m_settings.getSetting("NODE_PIC_OUTPUT");

            // This assumes that the .py file is executable,
            // and the first line has #!/usr/bin/python in it.
            execl(m_settings.getSetting("NODE_PI_CAM_LOCATION").c_str(),
                  "",                   // For some stupid reason, this is needed or the first argument is ignored :|
                  userAgentArg.c_str(),
                  uriArg.c_str(),
                  portArg.c_str(),
                  pictureOutputArg.c_str(),
                  nullptr);
            _exit(0);
        }
        // Parent Process
        else if (pid > 0) {
            Common::IO::ConsoleLogger::err.writeLine("Running CTSN Node...");
            m_shutdownCV.wait();
            // SIGINT will gracefully terminate the python process.
            ::kill(pid, SIGINT);
            wait();
        }
        else {
            throw std::runtime_error("Error creating picam process.");
        }
    }
    catch (const std::out_of_range &e) {
        Common::IO::ConsoleLogger::err.writeLineWithTimeStamp("Address out of range");
    }
    catch (const std::exception &e) {
        Common::IO::ConsoleLogger::err.writeLineWithTimeStamp(e.what());
    }

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

