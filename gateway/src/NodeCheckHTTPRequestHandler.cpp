#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeCheckEvent.h"
#include "gateway/NodeCheckHTTPRequestHandler.h"
#include "gateway/NodeContainerInterface.h"

namespace Gateway {

const std::string NodeCheckHTTPRequestHandler::POST_TRUE_MESSAGE =
    "Node check occurring";
const std::string NodeCheckHTTPRequestHandler::POST_FALSE_MESSAGE =
    "Node check not occurring";
const std::string NodeCheckHTTPRequestHandler::POST_MISSING_FIELD =
    "Missing Field";
const std::string NodeCheckHTTPRequestHandler::GET_MESSAGE = 
    "Usage: check=true";
const std::string NodeCheckHTTPRequestHandler::CHECK_FORM_DATA =
    "check";

NodeCheckHTTPRequestHandler::NodeCheckHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                                         MariaDBInterface *mariadb,
                                                         NodeContainerInterface *nodes) :

    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb),
    m_nodes(nodes)
{

}

NodeCheckHTTPRequestHandler::~NodeCheckHTTPRequestHandler() {

}

void NodeCheckHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request,
                                                    Poco::Net::HTTPServerResponse &response) {

    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &check = form[CHECK_FORM_DATA];

        if (check == "true") {
             std::shared_ptr<NodeCheckEvent> event(
                new NodeCheckEvent(m_eventExecutor,
                                   m_mariadb,
                                   m_nodes)
             );

             m_eventExecutor->addEvent(event);
             sendSuccessResponse(response, POST_TRUE_MESSAGE);
        }
        else {
            sendSuccessResponse(response, POST_FALSE_MESSAGE);
        }
    }
    catch(const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_MISSING_FIELD);
    }
}

void NodeCheckHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                   Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

