#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "gateway/BaseHTTPRequestHandler.h"

namespace Gateway {

BaseHTTPRequestHandler::BaseHTTPRequestHandler() {
    //ctor
}

BaseHTTPRequestHandler::~BaseHTTPRequestHandler() {
    //dtor
}

void BaseHTTPRequestHandler::sendSuccessResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "ACK"*/) {
    sendResponse(response, Poco::Net::HTTPResponse::HTTP_OK, message);
}

void BaseHTTPRequestHandler::sendNotFoundResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Not Found"*/) {
    sendResponse(response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND, message);
}

void BaseHTTPRequestHandler::sendBadRequestResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Bad Request"*/) {
    sendResponse(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, message);
}

void BaseHTTPRequestHandler::sendForbiddenResponse(Poco::Net::HTTPServerResponse &response, const std::string &message /*= "Forbidden"*/) {
    sendResponse(response, Poco::Net::HTTPResponse::HTTP_FORBIDDEN, message);
}

void BaseHTTPRequestHandler::sendResponse(Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse::HTTPStatus status, const std::string &message) {
    response.setContentType("text/html");
    response.setStatus(status);
    std::ostream &ostr = response.send();
    ostr << message;
}

}
