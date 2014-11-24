#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "EventExecutorInterface.h"
#include "picture_parser/CVRunnerInterface.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/PictureParseEvent.h"
#include "picture_parser/PictureParseHTTPRequestHandler.h"

namespace PictureParser {

const std::string PictureParseHTTPRequestHandler::POST_SUCCESS = 
    "Data send to be parsed";
const std::string PictureParseHTTPRequestHandler::POST_MISSING_FIELD =
    "Missing Field";
const std::string PictureParseHTTPRequestHandler::POST_INVALID_ARGUMENT = 
    "Invalid field info.";
const std::string PictureParseHTTPRequestHandler::GET_MESSAGE = 
    "Usage: node=x&part=y&data=base64(image)";
const std::string PictureParseHTTPRequestHandler::NODE_FORM_DATA = 
    "node";
const std::string PictureParseHTTPRequestHandler::PART_FORM_DATA =
    "part";
const std::string PictureParseHTTPRequestHandler::DATA_FORM_DATA =
    "data";

PictureParseHTTPRequestHandler::PictureParseHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                       PictureContainer *pc,
                                       CVRunnerInterface *cvRunner,
                                       CTSNCommon::HTTPPosterInterface *httpPoster) :
    m_eventExecutor(eventExecutor),
    m_pc(pc),
    m_cvRunner(cvRunner),
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

        unsigned int nodeID;
        unsigned int partNumber;

        try {
            size_t nodeStrSize;
            size_t partStrSize;
            nodeID = std::stoul(nodeStr, &nodeStrSize);
            partNumber = std::stoul(partStr, &partStrSize);

            if (nodeStrSize != nodeStr.size()) {
                throw std::invalid_argument(POST_INVALID_ARGUMENT);
            }
            else if (partStrSize != partStr.size()) {
                throw std::invalid_argument(POST_INVALID_ARGUMENT);
            }
        }
        // If stoi fails...
        catch (const std::exception &e) {
            throw std::invalid_argument(POST_INVALID_ARGUMENT);
        }

        std::shared_ptr<PictureParseEvent> event (new PictureParseEvent(nodeID,
                                                                        partNumber,
                                                                        dataStr,
                                                                        m_pc,
                                                                        m_cvRunner,
                                                                        m_httpPoster));
        m_eventExecutor->addEvent(event);
        sendSuccessResponse(response, POST_SUCCESS);
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_MISSING_FIELD);
    }
    catch(const std::exception &e) {
        sendBadRequestResponse(response, e.what());
    }
}

void PictureParseHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                      Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

