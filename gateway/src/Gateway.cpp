#include <functional>
#include <iostream>

#include "CTSNSharedGlobals.py"
#include "EventExecutor.h"
#include "gateway/EmailEvent.h"
#include "gateway/Gateway.h"
#include "gateway/TextMessageEvent.h"
#include "gateway/Uart.h"
#include "gateway/UartRecvThread.h"
#include "gateway/UartTxEvent.h"
#include "gateway/HTTPRequestFactory.h"
#include "io/InputReader.h"
#include "SMutex.h"

namespace Gateway {

void Gateway::RxSignal(int status) {
    getInstance().m_recvThread->dataReady();
}

Gateway &Gateway::getInstance() {
    static Gateway g;
    return g;
}

Gateway::Gateway() :
    m_eventExecutor(new Common::EventExecutor),
    m_input(&std::cin),
    m_output(&Common::IO::ConsoleLogger::out),
    m_uart(new Uart(RxSignal)),
    m_recvThread(new UartRecvThread(m_uart)),
    m_socket(nullptr),
    m_server(nullptr)
{
}

Gateway::~Gateway() {
    shutdown();
    delete m_server;
    delete m_socket;
    delete m_recvThread;
    delete m_uart;
    delete m_eventExecutor;
}

void Gateway::initHTTPServer() {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(100);
    params->setMaxThreads(2);

    m_socket = new Poco::Net::ServerSocket(GATEWAY_COMMAND_PORT);
    m_server = new Poco::Net::HTTPServer(new HTTPRequestFactory(this, m_eventExecutor, m_uart), *m_socket, params);
}

void Gateway::start() {

    try {
        initHTTPServer();
        m_server->start();

        try {
            m_uart->open("/dev/ttyAMA0");
            m_recvThread->start();
        }
        catch(const std::runtime_error &e) {
            m_output->writeLine(e.what());
        }

        m_output->writeLine("Running CTSN Gateway...");
        m_shutdownSemaphore.wait();

    }
    catch (const std::runtime_error &e) {
        m_output->writeLine(e.what());
    }

    m_server->stop();
    m_recvThread->kill();
    m_uart->close();
}

void Gateway::shutdown() {
    m_shutdownSemaphore.post();
}

}
