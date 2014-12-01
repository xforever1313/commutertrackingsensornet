#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/Node.h"
#include "gateway/NodeContainerInterface.h"
#include "ctsn_common/XBeeTxEvent.h"
#include "gateway/XBeeTxHTTPRequestHandler.h"

namespace Gateway {

const std::string XBeeTxHTTPRequestHandler::POST_MESSAGE = "Message send to node";
const std::string XBeeTxHTTPRequestHandler::POST_MISSING_FIELD_MESSAGE = "Missing Field!";
const std::string XBeeTxHTTPRequestHandler::GET_MESSAGE = "Usage: message=message_to_send_to_xbee&node=id";
const std::string XBeeTxHTTPRequestHandler::MESSAGE_FORM_DATA = "message";
const std::string XBeeTxHTTPRequestHandler::NODE_FORM_DATA = "node";

XBeeTxHTTPRequestHandler::XBeeTxHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                                   CTSNCommon::UartInterface *uart,
                                                   NodeContainerInterface *nodes) :
    m_eventExecutor(eventExecutor),
    m_uart(uart),
    m_nodes(nodes)
{    

}

XBeeTxHTTPRequestHandler::~XBeeTxHTTPRequestHandler() {

}

void XBeeTxHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

void XBeeTxHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, 
                                                 Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &message = form[MESSAGE_FORM_DATA];
        const std::string &nodeStr = form[NODE_FORM_DATA];

        CTSNCommon::Node node = m_nodes->convertStringToNode(nodeStr);

        m_eventExecutor->addEvent(std::shared_ptr<Common::EventInterface>(new CTSNCommon::XBeeTxEvent(node, message, m_uart)));

        sendSuccessResponse(response, POST_MESSAGE);
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_MISSING_FIELD_MESSAGE);
    }
    catch (const std::exception &e) {
        sendBadRequestResponse(response, e.what());
    }
}

}

