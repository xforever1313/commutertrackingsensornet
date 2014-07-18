#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "CTSNSharedGlobals.py"
#include "gateway/HTTPRequestFactory.h"
#include "gateway/NotFoundHTTPRequestHandler.h"
#include "gateway/ShutdownHTTPRequestHandler.h"
#include "Secrets.py"

namespace Gateway {

const std::string HTTPRequestFactory::INVALID_USER_AGENT = "Invalid user";

HTTPRequestFactory::HTTPRequestFactory(ShutdownInterface *shutdown) :
    m_shutdown(shutdown)
{
}

HTTPRequestFactory::~HTTPRequestFactory() {

}

Poco::Net::HTTPRequestHandler *HTTPRequestFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
    // Poco handles deleting the new
    if (request.getURI() == SHUTDOWN_URI) {
        return new ShutdownHTTPRequestHandler(m_shutdown);
    }

    else {
        return new NotFoundHTTPRequestHandler();
    }
}

}
