#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <stdexcept>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorHTTPRequestHandler.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/Node.h"
#include "gateway/NodeContainer.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

const std::string ErrorHTTPRequestHandler::GET_MESSAGE = "Usage:\nnode=x&message=y";
const std::string ErrorHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE = "Missing field.";
const std::string ErrorHTTPRequestHandler::POST_SUCCESS_MESSAGE = "Error Message Sent.";
const std::string ErrorHTTPRequestHandler::NODE_FORM_DATA = "node";
const std::string ErrorHTTPRequestHandler::MESSAGE_FORM_DATA = "message";

ErrorHTTPRequestHandler::ErrorHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                                 MariaDBInterface *mariadb) :
    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb)
{

}

ErrorHTTPRequestHandler::~ErrorHTTPRequestHandler() {

}

void ErrorHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &nodeStr = form[NODE_FORM_DATA];
        const std::string &messageStr =  form[MESSAGE_FORM_DATA];

        Node node = NodeContainer::convertStringToNode(nodeStr);
        ErrorNumber messageType = ErrorMessage::convertStringToMessage(messageStr);

        std::shared_ptr<ErrorEvent> event(new ErrorEvent(messageType, node, m_mariadb));
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

void ErrorHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}
