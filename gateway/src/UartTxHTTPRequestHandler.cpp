#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/UartTxEvent.h"
#include "gateway/UartTxHTTPRequestHandler.h"

namespace Gateway {

const std::string UartTxHTTPRequestHandler::POST_MESSAGE = "TX Uart Message Queued";
const std::string UartTxHTTPRequestHandler::GET_MESSAGE = "Usage: message=message_to_send_to_uart";
const std::string UartTxHTTPRequestHandler::MESSAGE_FORM_DATA = "message";

UartTxHTTPRequestHandler::UartTxHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor, UartInterface *uart) :
    m_eventExecutor(eventExecutor),
    m_uart(uart)
{
}

UartTxHTTPRequestHandler::~UartTxHTTPRequestHandler() {
}

void UartTxHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

void UartTxHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        std::string message = form[MESSAGE_FORM_DATA];
        m_eventExecutor->addEvent(std::shared_ptr<Common::EventInterface>(new UartTxEvent(message, m_uart)));

        sendSuccessResponse(response, POST_MESSAGE);

    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, "Message not found");
    }
}

}
