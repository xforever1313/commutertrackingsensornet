#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "gateway/BadClientHTTPRequestHandler.h"

namespace Gateway {

const std::string BadClientHTTPRequestHandler::MESSAGE = "Forbidden:  Please use a verifed client.";

BadClientHTTPRequestHandler::BadClientHTTPRequestHandler() {
}

BadClientHTTPRequestHandler::~BadClientHTTPRequestHandler() {

}

void BadClientHTTPRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendForbiddenResponse(response, MESSAGE);
}

}
