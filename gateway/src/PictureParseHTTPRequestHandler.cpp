#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "EventExecutorInterface.h"
#include "gateway/PictureParseEvent.h"
#include "gateway/PictureParseHTTPRequestHandler.h"

namespace Gateway {

const std::string PictureParseHTTPRequestHandler::POST_SUCCESS =
    "Data sent to picture parser";
const std::string PictureParseHTTPRequestHandler::POST_MISSING_FIELD = 
    "Missing data field";
const std::string PictureParseHTTPRequestHandler::GET_MESSAGE =
    "Usage: node=x&part=y&z=base64(data)";
const std::string PictureParseHTTPRequestHandler::NODE_FORM_DATA = 
    "node";
const std::string PictureParseHTTPRequestHandler::PART_FORM_DATA =
    "part";
const std::string PictureParseHTTPRequestHandler::DATA_FORM_DATA = 
    "data";

PictureParseHTTPRequestHandler::PictureParseHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                       CTSNCommon::HTTPPosterInterface *httpPoster) :

    m_eventExecutor(eventExecutor),
    m_httpPoster(httpPoster)
{

}

PictureParseHTTPRequestHandler::~PictureParseHTTPRequestHandler() {

}

void PictureParseHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request,
                                                       Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &nodeStr = form[NODE_FORM_DATA];
        const std::string &partStr = form[PART_FORM_DATA];
        const std::string &dataStr = form[DATA_FORM_DATA];

        std::shared_ptr<PictureParseEvent> event (new PictureParseEvent(nodeStr,
                                                                        partStr,
                                                                        dataStr,
                                                                        m_httpPoster));
        m_eventExecutor->addEvent(event);
        sendSuccessResponse(response, POST_SUCCESS);
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_MISSING_FIELD);
    }
}

void PictureParseHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                      Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

