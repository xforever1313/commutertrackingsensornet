#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "CTSNSharedGlobals.py"
#include "gateway/BadClientHTTPRequestHandler.h"
#include "gateway/HTTPRequestFactory.h"
#include "gateway/NotFoundHTTPRequestHandler.h"
#include "gateway/ShutdownHTTPRequestHandler.h"
#include "gateway/TextMessageHTTPRequestHandler.h"
#include "gateway/UartTxHTTPRequestHandler.h"
#include "Secrets.py"

#include <iostream>

namespace Gateway {

const std::string HTTPRequestFactory::INVALID_USER_AGENT = "Invalid user";

HTTPRequestFactory::HTTPRequestFactory(ShutdownInterface *shutdown, Common::EventExecutorInterface *eventExecutor, UartInterface *uart) :
    m_shutdown(shutdown),
    m_eventExecutor(eventExecutor),
    m_uart(uart)
{
}

HTTPRequestFactory::~HTTPRequestFactory() {

}

Poco::Net::HTTPRequestHandler *HTTPRequestFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
    std::string userAgent = request.get("user-agent", INVALID_USER_AGENT);

    // Poco handles deleting the new
    if (userAgent == INVALID_USER_AGENT) {
        return new BadClientHTTPRequestHandler();
    }
    else if (userAgent != USER_AGENT) {
        return new BadClientHTTPRequestHandler();
    }
    else if (request.getURI() == UART_TX_URI) {
        return new UartTxHTTPRequestHandler(m_eventExecutor, m_uart);
    }
    else if (request.getURI() == SHUTDOWN_URI) {
        return new ShutdownHTTPRequestHandler(m_shutdown);
    }
    else if (request.getURI() == TEXT_MESSAGE_URI) {
        return new TextMessageHTTPRequestHandler(m_eventExecutor);
    }
    else {
        return new NotFoundHTTPRequestHandler();
    }
}

}
