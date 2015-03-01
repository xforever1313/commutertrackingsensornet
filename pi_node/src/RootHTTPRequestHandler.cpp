#include <string>

#include "pi_node/RootHTTPRequestHandler.h"

namespace PiNode {

const std::string RootHTTPRequestHandler::RESPONSE = "CTSN Node Running";

RootHTTPRequestHandler::RootHTTPRequestHandler()
{
}

RootHTTPRequestHandler::~RootHTTPRequestHandler() {

}

void RootHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request,
                                               Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, RESPONSE);
}

void RootHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                              Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, RESPONSE);
}

}

