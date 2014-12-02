#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/UartInterface.h"
#include "pi_node/PicSendEvent.h"
#include "pi_node/PicSendHTTPRequestHandler.h"

namespace PiNode {

PicSendHTTPRequestHandler::PicSendHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                                     CTSNCommon::UartInterface *uart) :
    m_eventExecutor(eventExecutor),
    m_uart(uart)
{

}

PicSendHTTPRequestHandler::~PicSendHTTPRequestHandler() {

}

void PicSendHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, 
                                                  Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &pictureString = form["picture_file"];

        std::shared_ptr<PicSendEvent> event (new PicSendEvent(pictureString, m_uart));
        m_eventExecutor->addEvent(event);

        sendSuccessResponse(response, "Picture Queued");
   }
   catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, "Missing Field");
   }
}

void PicSendHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                 Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, "Just post the thing");
}

}

