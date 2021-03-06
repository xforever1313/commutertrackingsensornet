#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "CTSNSharedGlobals.py"
#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/SettingsParser.h"
#include "gateway/DatabasePokeHTTPRequestHandler.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "gateway/EmailHTTPRequestHandler.h"
#include "gateway/ErrorHTTPRequestHandler.h"
#include "gateway/HTTPRequestFactory.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/LogMessageHTTPRequestHandler.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "gateway/NodeCheckHTTPRequestHandler.h"
#include "gateway/NodeStatusUpdateHTTPRequestHandler.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "gateway/RootHTTPRequestHandler.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "gateway/TextMessageHTTPRequestHandler.h"
#include "ctsn_common/XBeeTxHTTPRequestHandler.h"
#include "gateway/UartTxHTTPRequestHandler.h"

#include <iostream>

namespace Gateway {

const std::string HTTPRequestFactory::INVALID_USER_AGENT = "Invalid user";

HTTPRequestFactory::HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown,
                                       Common::EventExecutorInterface *eventExecutor,
                                       CTSNCommon::UartInterface *uart,
                                       MariaDBInterface *mariadb,
                                       CTSNCommon::NodeContainerInterface *nodes,
                                       CTSNCommon::HTTPPosterInterface *httpPoster) :
    m_shutdown(shutdown),
    m_eventExecutor(eventExecutor),
    m_uart(uart),
    m_mariadb(mariadb),
    m_nodes(nodes),
    m_httpPoster(httpPoster),
    m_settings(CTSNCommon::Settings::getInstance())
{
}

HTTPRequestFactory::~HTTPRequestFactory() {

}

Poco::Net::HTTPRequestHandler *HTTPRequestFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
    std::string userAgent = request.get("user-agent", INVALID_USER_AGENT);

    // Poco handles deleting the new
    if (userAgent == INVALID_USER_AGENT) {
        return new CTSNCommon::BadClientHTTPRequestHandler();
    }
    else if (userAgent != m_settings.getSetting("GATEWAY_AGENT")) {
        return new CTSNCommon::BadClientHTTPRequestHandler();
    }
    else if (request.getURI() == ROOT_URI) {
        return new RootHTTPRequestHandler();
    }
    else if (request.getURI() == DATABASE_POKE_URI) {
        return new DatabasePokeHTTPRequestHandler(m_mariadb, m_eventExecutor);
    }
    else if (request.getURI() == DATA_RESULT_URI) {
        return new DataHTTPRequestHandler(m_eventExecutor, m_mariadb, m_nodes);
    }
    else if (request.getURI() == NODE_CHECK_URI) {
        return new NodeCheckHTTPRequestHandler(m_eventExecutor, m_mariadb, m_nodes);
    }
    else if (request.getURI() == XBEE_TX_URI) {
        return new CTSNCommon::XBeeTxHTTPRequestHandler(m_eventExecutor, m_uart, m_nodes);
    }
    else if (request.getURI() == NODE_STATUS_UPDATE_URI) {
        return new NodeStatusUpdateHTTPRequestHandler(m_eventExecutor, m_nodes, m_mariadb);
    }
    else if (request.getURI() == UART_TX_URI) {
        return new UartTxHTTPRequestHandler(m_eventExecutor, m_uart);
    }
    else if (request.getURI() == ERROR_MESSAGE_URI) {
        return new ErrorHTTPRequestHandler(m_eventExecutor, m_mariadb, m_nodes);
    }
    else if (request.getURI() == SHUTDOWN_URI) {
        return new CTSNCommon::ShutdownHTTPRequestHandler(m_shutdown);
    }
    else if (request.getURI() == TEXT_MESSAGE_URI) {
        return new TextMessageHTTPRequestHandler(m_eventExecutor);
    }
    else if (request.getURI() == EMAIL_URI) {
        return new EmailHTTPRequestHandler(m_eventExecutor);
    }
    else if (request.getURI() == LOG_MESSAGE_URI) {
        return new LogMessageHTTPRequestHandler(m_eventExecutor, m_mariadb, m_nodes);
    }
    else {
        return new CTSNCommon::NotFoundHTTPRequestHandler();
    }
}

}
