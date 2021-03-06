#include <functional>
#include <iostream>
#include <mutex>

#include "ctsn_common/HTTPPoster.h"
#include "CTSNSharedGlobals.py"
#include "EventExecutor.h"
#include "gateway/EmailEvent.h"
#include "gateway/Gateway.h"
#include "gateway/MariaDBWrapper.h"
#include "gateway/NodeContainer.h"
#include "gateway/TextMessageEvent.h"
#include "ctsn_common/Uart.h"
#include "ctsn_common/UartRecvThread.h"
#include "ctsn_common/XBeeCallbacks.h"
#include "gateway/HTTPRequestFactory.h"
#include "io/InputReader.h"

namespace Gateway {

void Gateway::RxSignal(int status) {
    getInstance().m_recvThread->dataReady();
}

Gateway &Gateway::getInstance() {
    static Gateway g;
    return g;
}

Gateway::Gateway() :
    m_settings(CTSNCommon::Settings::getInstance()),
    m_eventExecutor(new Common::EventExecutor),
    m_input(&std::cin),
    m_output(&Common::IO::ConsoleLogger::out),
    m_uart(new CTSNCommon::Uart(RxSignal)),
    m_xbeeCallbacks(new CTSNCommon::XBeeCallbacks(m_settings.getShortSetting("GATEWAY_PORT"), m_settings.getSetting("GATEWAY_AGENT"))),
    m_xbeeController(new CTSNCommon::XBeeController(m_xbeeCallbacks)),
    m_recvThread(new CTSNCommon::UartRecvThread(m_uart, m_xbeeController)),
    m_socket(nullptr),
    m_server(nullptr),
    m_mariadb(nullptr),
    m_nodes(nullptr),
    m_httpPoster(new CTSNCommon::HTTPPoster(m_settings.getSetting("GATEWAY_AGENT")))
{
}

Gateway::~Gateway() {
    shutdown();
    delete m_server;
    delete m_socket;
    delete m_recvThread;
    delete m_xbeeController;
    delete m_xbeeCallbacks;
    delete m_uart;
    delete m_eventExecutor;
    delete m_mariadb; //Delete this last, as some left over events may use it.
    delete m_nodes;
    delete m_httpPoster;
}

void Gateway::initHTTPServer() {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(500);
    params->setMaxThreads(1);

    m_socket = new Poco::Net::ServerSocket(m_settings.getShortSetting("GATEWAY_PORT"));
    m_server = new Poco::Net::HTTPServer(new HTTPRequestFactory(this, m_eventExecutor, m_uart, m_mariadb, m_nodes, m_httpPoster),
                                         *m_socket, params);
}

void Gateway::initMariaDB() {
    m_mariadb = new MariaDBWrapper();
    m_mariadb->mysql_init();
    m_mariadb->mysql_real_connect(m_settings.getSetting("MARIADB_IP").c_str(),
                                  m_settings.getSetting("MARIADB_USER").c_str(),
                                  m_settings.getSetting("MARIADB_PASS").c_str(),
                                  m_settings.getSetting("MARIADB_NAME").c_str(),
                                  m_settings.getShortSetting("MARIADB_PORT"),
                                  nullptr,
                                  0);
}

void Gateway::start() {

    // start event executor
    m_eventExecutor->startExecutor();

    bool HTTPServerStarted = false;
    try {
        // Mariadb MUST come before the http server.
        initMariaDB();

        //Create the nodes.
        m_nodes = new NodeContainer(m_mariadb);
        m_nodes->refreshNodes();

        initHTTPServer();
        m_server->start();
        HTTPServerStarted = true;

        try {
            m_uart->open(m_settings.getSetting("GATEWAY_SERIAL"));
            m_xbeeController->start();
            m_recvThread->start();
        }
        catch(const std::runtime_error &e) {
            m_output->writeLineWithTimeStamp(e.what());
        }

        m_output->writeLine("Running CTSN Gateway...");
        m_shutdownSemaphore.wait();

    }
    catch (const std::runtime_error &e) {
        m_output->writeLineWithTimeStamp(e.what());
    }

    // Ensure the server is started, or a segfault occurs.
    if (HTTPServerStarted) {
        m_server->stop();
    }
    m_recvThread->kill();
    m_xbeeController->kill();
    m_uart->close();
}

void Gateway::shutdown() {
    m_shutdownSemaphore.post();
}

}

