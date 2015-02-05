#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/GPIOControllerInterface.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/UartInterface.h"
#include "EventExecutorInterface.h"
#include "pi_node/BatteryCheckEvent.h"
#include "pi_node/BatteryCheckHTTPRequestHandler.h"

namespace PiNode {

const std::string BatteryCheckHTTPRequestHandler::GET_MESSAGE =
    "Usage: check=true";
const std::string BatteryCheckHTTPRequestHandler::POST_SUCCESS =
    "Checking battery";
const std::string BatteryCheckHTTPRequestHandler::POST_FALSE_MESSAGE =
    "Won't check battery";
const std::string BatteryCheckHTTPRequestHandler::CHECK_FORM_DATA =
    "check";

BatteryCheckHTTPRequestHandler::BatteryCheckHTTPRequestHandler(CTSNCommon::NodeContainerInterface *nodes,
                                                               Common::EventExecutorInterface *eventExecutor,
                                                               CTSNCommon::GPIOControllerInterface &gpio,
                                                               CTSNCommon::UartInterface *uart) :
    m_nodes(nodes),
    m_eventExecutor(eventExecutor),
    m_gpio(gpio),
    m_uart(uart)
{
}

BatteryCheckHTTPRequestHandler::~BatteryCheckHTTPRequestHandler() {

}

void BatteryCheckHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request,
                                                       Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        if (form[CHECK_FORM_DATA] == "true") {

            std::shared_ptr<BatteryCheckEvent> event (new BatteryCheckEvent(m_nodes, m_eventExecutor, m_gpio, m_uart));
            m_eventExecutor->addEvent(event);

            sendSuccessResponse(response, POST_SUCCESS);
        }
        else {
            sendSuccessResponse(response, POST_FALSE_MESSAGE);
        }
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response);
    }
}

void BatteryCheckHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                      Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}
