#include <string>

#include "gateway/RootHTTPRequestHandler.h"

namespace Gateway {

const std::string RootHTTPRequestHandler::RESPONSE = "CTSN Gateway Running";

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

