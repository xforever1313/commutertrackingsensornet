#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutor.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeStatusUpdateEvent.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "gateway/NodeStatusUpdateHTTPRequestHandler.h"

namespace Gateway { 

const std::string NodeStatusUpdateHTTPRequestHandler::GET_MESSAGE = 
    "Usage: node=x&status=y";
const std::string NodeStatusUpdateHTTPRequestHandler::POST_SUCCESS_MESSAGE =
    "Node status updated";
const std::string NodeStatusUpdateHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE = 
    "Missing Field";
const std::string NodeStatusUpdateHTTPRequestHandler::NODE_FORM_DATA = "node";
const std::string NodeStatusUpdateHTTPRequestHandler::STATUS_FORM_DATA = "status";

NodeStatusUpdateHTTPRequestHandler::NodeStatusUpdateHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                           NodeContainerInterface *nodes,
                                           MariaDBInterface *mariadb) :
    m_eventExecutor(eventExecutor),
    m_nodes(nodes),
    m_mariadb(mariadb)
{

}

NodeStatusUpdateHTTPRequestHandler::~NodeStatusUpdateHTTPRequestHandler() {

}

void NodeStatusUpdateHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, 
                                                           Poco::Net::HTTPServerResponse &response) {

   try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &nodeStr = form[NODE_FORM_DATA];
        const std::string &statusStr = form[STATUS_FORM_DATA];

        const CTSNCommon::Node::NodeStatus status = 
            CTSNCommon::Node::convertStringToNodeStatus(statusStr);
        const CTSNCommon::Node node = m_nodes->convertStringToNode(nodeStr);

        std::shared_ptr<NodeStatusUpdateEvent> event (
            new NodeStatusUpdateEvent(status, node.getID(), m_nodes, m_eventExecutor, m_mariadb));

        m_eventExecutor->addEvent(event);

        sendSuccessResponse(response, POST_SUCCESS_MESSAGE);
   }
   catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_MISSING_FIELD_MESSAGE);
   }
   catch(const std::exception &e) {
        sendBadRequestResponse(response, e.what());
   }
}

void NodeStatusUpdateHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                          Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}



}

