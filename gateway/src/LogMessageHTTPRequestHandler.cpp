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
#include "gateway/Node.h"
#include "gateway/NodeContainerInterface.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

const std::string LogMessageHTTPRequestHandler::GET_MESSAGE = "Usage:\nnode=x&message=y";
const std::string LogMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE = "Missing field.";
const std::string LogMessageHTTPRequestHandler::POST_SUCCESS_MESSAGE = "Message Logged.";
const std::string LogMessageHTTPRequestHandler::NODE_FORM_DATA = "node";
const std::string LogMessageHTTPRequestHandler::MESSAGE_FORM_DATA = "message";

LogMessageHTTPRequestHandler::LogMessageHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                                           MariaDBInterface *mariadb,
                                                           NodeContainerInterface *nodeContainer) :
    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb),
    m_nodes(nodeContainer)
{

}

LogMessageHTTPRequestHandler::~LogMessageHTTPRequestHandler() {

}

void LogMessageHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &nodeStr = form[NODE_FORM_DATA];
        const std::string &messageStr =  form[MESSAGE_FORM_DATA];

        ErrorNumber messageType = ErrorMessage::convertStringToMessage(messageStr);
        const Node node = m_nodes->convertStringToNode(nodeStr);

        std::shared_ptr<LogEvent> event(new LogEvent(messageType, node, m_mariadb));
        m_eventExecutor->addEvent(event);

        sendSuccessResponse(response, POST_SUCCESS_MESSAGE);
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_FAILURE_MISSING_FIELD_MESSAGE);
    }
    catch (const std::exception &e) {
        sendBadRequestResponse(response, e.what());
    }
}

void LogMessageHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

