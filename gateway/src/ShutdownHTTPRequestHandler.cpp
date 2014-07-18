#include "gateway/ShutdownHTTPRequestHandler.h"
#include "gateway/ShutdownInterface.h"

namespace Gateway {

const std::string ShutdownHTTPRequestHandler::MESSAGE = "Shutting down gateway...";

ShutdownHTTPRequestHandler::ShutdownHTTPRequestHandler(ShutdownInterface *shutdown) :
    m_shutdown(shutdown)
{
}

ShutdownHTTPRequestHandler::~ShutdownHTTPRequestHandler() {
}

void ShutdownHTTPRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    m_shutdown->shutdown();
    sendSuccessResponse(response, MESSAGE);
}

}
