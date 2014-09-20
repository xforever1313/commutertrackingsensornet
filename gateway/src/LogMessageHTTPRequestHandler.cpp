#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <stdexcept>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/Gateway.h"
#include "gateway/LogEvent.h"
#include "gateway/LogMessageHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

const std::string LogMessageHTTPRequestHandler::GET_MESSAGE = "Usage:\nnode=x&message=y";
const std::string LogMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE = "Missing field.";
const std::string LogMessageHTTPRequestHandler::POST_FAILURE_INVALID_NODE = "Node does not exist.";
const std::string LogMessageHTTPRequestHandler::POST_FAILURE_INVALID_MESSAGE = "Invalid error message." ;
const std::string LogMessageHTTPRequestHandler::POST_SUCCESS_MESSAGE = "Message Logged.";
const std::string LogMessageHTTPRequestHandler::NODE_FORM_DATA = "node";
const std::string LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA = "message";

LogMessageHTTPRequestHandler::LogMessageHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                                           MariaDBInterface *mariadb) :
    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb)
{

}

LogMessageHTTPRequestHandler::~LogMessageHTTPRequestHandler() {

}

void LogMessageHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &nodeStr = form[NODE_FORM_DATA];
        const std::string &messageStr =  form[MESSAGE_FORM_DATA];

        unsigned int nodeNumber = convertStringToNodeNumber(nodeStr);
        ErrorNumber messageType = ErrorMessage::convertStringToMessage(messageStr);

        std::shared_ptr<LogEvent> event(new LogEvent(messageType, nodeNumber, m_mariadb));
        m_eventExecutor->addEvent(event);

        sendSuccessResponse(response, POST_SUCCESS_MESSAGE);
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_FAILURE_MISSING_FIELD_MESSAGE);
    }
    catch (const std::out_of_range &e) {
        sendBadRequestResponse(response, e.what());
    }
    catch (const std::invalid_argument &e) {
        sendBadRequestResponse(response, e.what());
    }
}

void LogMessageHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

unsigned int LogMessageHTTPRequestHandler::convertStringToNodeNumber(const std::string &nodeString) {
    unsigned int nodeNumber = 0;
    try {
        size_t stringSize;
        nodeNumber = std::stoi(nodeString, &stringSize);

        // Ensure theres nothing left over from the nodeString.
        if (stringSize != nodeString.size()) {
            throw std::invalid_argument(POST_FAILURE_INVALID_NODE);
        }
        // Node can never be zero.
        else if (nodeNumber == 0) {
            throw std::out_of_range(POST_FAILURE_INVALID_NODE);
        }
        else if (nodeNumber > Gateway::getNumberOfNodes()) {
            throw std::out_of_range(POST_FAILURE_INVALID_NODE);
        }
    }
    catch (const std::invalid_argument &e) {
        throw std::invalid_argument(POST_FAILURE_INVALID_NODE);
    }

    return nodeNumber;
} 

}

