#include <Poco/Net/HTTPServer.h>

#include "CTSNSharedGlobals.py"
#include "EventExecutor.h"
#include "io/ConsoleLogger.h"
#include "picture_parser/HTTPRequestFactory.h"
#include "picture_parser/PictureParser.h"

namespace PictureParser {

PictureParser &PictureParser::getInstance() {
    static PictureParser parser;
    return parser;
}

PictureParser::PictureParser() :
    m_eventExecutor(new Common::EventExecutor())
{

}

PictureParser::~PictureParser() {
    delete m_server;
    delete m_socket;
    delete m_eventExecutor;
}

void PictureParser::initHTTPServer() {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(100);
    params->setMaxThreads(2);

    m_socket = new Poco::Net::ServerSocket(PICTURE_PARSER_COMMAND_PORT);
    m_server = new Poco::Net::HTTPServer(new HTTPRequestFactory(this, m_eventExecutor),
                                         *m_socket, params);
}

void PictureParser::start() {
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

void PictureParser::shutdown() {
    m_shutdownSemaphore.shutdown();
}

}

