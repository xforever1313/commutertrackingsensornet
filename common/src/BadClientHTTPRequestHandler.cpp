#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "ctsn_common/BadClientHTTPRequestHandler.h"

namespace CTSNCommon {

const std::string BadClientHTTPRequestHandler::MESSAGE = "Forbidden:  Please use a verifed client.";

BadClientHTTPRequestHandler::BadClientHTTPRequestHandler() {
}

BadClientHTTPRequestHandler::~BadClientHTTPRequestHandler() {

}

void BadClientHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    handleHTTPRequest(request, response);
}

void BadClientHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    handleHTTPRequest(request, response);
}

void BadClientHTTPRequestHandler::handleHTTPRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendForbiddenResponse(response, MESSAGE);
}

}
