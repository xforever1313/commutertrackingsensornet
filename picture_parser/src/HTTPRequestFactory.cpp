#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "CTSNSharedGlobals.py"
#include "picture_parser/HTTPRequestFactory.h"
#include "Secrets.py"

namespace PictureParser {

const std::string HTTPRequestFactory::INVALID_USER_AGENT = "Invalid user";

HTTPRequestFactory::HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown, 
                                       Common::EventExecutorInterface *eventExecutor) :
    m_shutdown(shutdown),
    m_eventExecutor(eventExecutor)
{

}

HTTPRequestFactory::~HTTPRequestFactory() {

}

Poco::Net::HTTPRequestHandler *HTTPRequestFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {

    const std::string userAgent = 
        request.get("user-agent", INVALID_USER_AGENT);

    // Poco handles deleting the new
    if (userAgent == INVALID_USER_AGENT) {
        return new CTSNCommon::BadClientHTTPRequestHandler();
    }
    else if ((userAgent != USER_AGENT) && (userAgent != PICTURE_PARSER_USER_AGENT)) {
        return new CTSNCommon::BadClientHTTPRequestHandler();
    }
    else if (request.getURI() == SHUTDOWN_URI) {
        return new CTSNCommon::ShutdownHTTPRequestHandler(m_shutdown);
    }
    else {
        return new CTSNCommon::NotFoundHTTPRequestHandler();
    }
}

}

