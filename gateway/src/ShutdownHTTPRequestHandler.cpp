#include "gateway/ShutdownHTTPRequestHandler.h"
#include "gateway/ShutdownInterface.h"

namespace Gateway {

const std::string ShutdownHTTPRequestHandler::POST_MESSAGE = "Shutting down gateway...";
const std::string ShutdownHTTPRequestHandler::GET_MESSAGE = "Form data:\n shutdown=true\tShut down the gateway";

ShutdownHTTPRequestHandler::ShutdownHTTPRequestHandler(ShutdownInterface *shutdown) :
    m_shutdown(shutdown)
{
}

ShutdownHTTPRequestHandler::~ShutdownHTTPRequestHandler() {
}

void ShutdownHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    m_shutdown->shutdown();
    sendSuccessResponse(response, POST_MESSAGE);
}

void ShutdownHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}
