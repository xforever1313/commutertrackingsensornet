#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "ctsn_common/UartInterface.h"
#include "CTSNSharedGlobals.py"
#include "pi_node/HTTPRequestFactory.h"
#include "pi_node/PicSendHTTPRequestHandler.h"
#include "Secrets.py"

namespace PiNode {

const std::string HTTPRequestFactory::INVALID_USER_AGENT = "Invalid user";

HTTPRequestFactory::HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown, 
                                       Common::EventExecutorInterface *eventExecutor,
                                       CTSNCommon::UartInterface *uart) :
    m_shutdown(shutdown),
    m_eventExecutor(eventExecutor),
    m_uart(uart)
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
    else if (userAgent != PI_NODE_USER_AGENT) {
        return new CTSNCommon::BadClientHTTPRequestHandler();
    }
    else if (request.getURI() == PICTURE_SEND_URI) {
        return new PicSendHTTPRequestHandler(m_eventExecutor, m_uart);
    }
    else if (request.getURI() == SHUTDOWN_URI) {
        return new CTSNCommon::ShutdownHTTPRequestHandler(m_shutdown);
    }
    else {
        return new CTSNCommon::NotFoundHTTPRequestHandler();
    }
}

}

