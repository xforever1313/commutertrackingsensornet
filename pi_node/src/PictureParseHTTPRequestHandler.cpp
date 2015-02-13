#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "pi_node/PictureParseHTTPRequestHandler.h"

namespace PiNode {

PictureParseHTTPRequestHandler::PictureParseHTTPRequestHandler(Common::EventExecutorInterface *cvExecutor) :
    m_cvExecutor(cvExecutor)
{
}

PictureParseHTTPRequestHandler::~PictureParseHTTPRequestHandler() {
}

void PictureParseHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request,
                                                       Poco::Net::HTTPServerResponse &response) {


}

void PictureParseHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                                      Poco::Net::HTTPServerResponse &response) {


}

}
