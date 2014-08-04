#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "gateway/NotFoundHTTPRequestHandler.h"

namespace Gateway {

const std::string NotFoundHTTPRequestHandler::MESSAGE = "404 - Not Found";

NotFoundHTTPRequestHandler::NotFoundHTTPRequestHandler() {
}

NotFoundHTTPRequestHandler::~NotFoundHTTPRequestHandler() {

}

void NotFoundHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    handleHTTPRequest(request, response);
}

void NotFoundHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    handleHTTPRequest(request, response);
}

void NotFoundHTTPRequestHandler::handleHTTPRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendNotFoundResponse(response, MESSAGE);
}

}
