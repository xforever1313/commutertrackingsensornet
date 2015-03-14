#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "pi_node/PictureParseEvent.h"
#include "pi_node/PictureParseHTTPRequestHandler.h"

namespace PiNode {

const std::string PictureParseHTTPRequestHandler::GET_MESSAGE = "usage: picture=/picture/location&remove=true";
const std::string PictureParseHTTPRequestHandler::POST_SUCCESS = "Parsing Picture";
const std::string PictureParseHTTPRequestHandler::POST_FAILURE = "Missing field.";
const std::string PictureParseHTTPRequestHandler::REMOVE_FORM_DATA = "remove";
const std::string PictureParseHTTPRequestHandler::PICTURE_FORM_DATA = "picture";

PictureParseHTTPRequestHandler::PictureParseHTTPRequestHandler(CTSNCommon::NodeContainerInterface *nodes,
                                                               CTSNCommon::UartInterface *uart,
                                                               Common::EventExecutorInterface *cvExecutor,
                                                               Common::EventExecutorInterface *eventExecutor) :
    m_nodes(nodes),
    m_uart(uart),
    m_cvExecutor(cvExecutor),
    m_eventExecutor(eventExecutor)
{
}

PictureParseHTTPRequestHandler::~PictureParseHTTPRequestHandler() {
}

void PictureParseHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request,
                                                       Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &removePicture = form[REMOVE_FORM_DATA];
        const std::string &pictureLocation = form[PICTURE_FORM_DATA];

        bool deletePicture = ((removePicture == "true") ? true : false );

        std::shared_ptr<PictureParseEvent> event (new PictureParseEvent(m_nodes, m_uart, pictureLocation, deletePicture, m_eventExecutor));
        m_cvExecutor->addEvent(event);

        sendSuccessResponse(response, POST_SUCCESS);
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_FAILURE);
    }
}

void PictureParseHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                      Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}
