#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "ctsn_common/ShutdownInterface.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"

namespace CTSNCommon {

const std::string ShutdownHTTPRequestHandler::POST_TRUE_MESSAGE = "Shutting down...";
const std::string ShutdownHTTPRequestHandler::POST_FALSE_MESSAGE = "Will not shutdown.";
const std::string ShutdownHTTPRequestHandler::GET_MESSAGE = "Form data:\n\tshutdown=true\tShut down the program";
const std::string ShutdownHTTPRequestHandler::SHUTDOWN_FORM_DATA = "shutdown";

ShutdownHTTPRequestHandler::ShutdownHTTPRequestHandler(CTSNCommon::ShutdownInterface *shutdown) :
    m_shutdown(shutdown)
{
}

ShutdownHTTPRequestHandler::~ShutdownHTTPRequestHandler() {
}

void ShutdownHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {

    try {
        Poco::Net::HTMLForm form(request, request.stream());
        if (form[SHUTDOWN_FORM_DATA] == "true") {
            m_shutdown->shutdown();
            sendSuccessResponse(response, POST_TRUE_MESSAGE);
        }
        else {
            sendSuccessResponse(response, POST_FALSE_MESSAGE);
        }
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response);
    }
}

void ShutdownHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}
