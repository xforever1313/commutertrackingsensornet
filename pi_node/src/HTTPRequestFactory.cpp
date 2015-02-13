#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "ctsn_common/BadClientHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/NotFoundHTTPRequestHandler.h"
#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/ShutdownHTTPRequestHandler.h"
#include "ctsn_common/UartInterface.h"
#include "CTSNSharedGlobals.py"
#include "pi_node/BatteryCheckHTTPRequestHandler.h"
#include "pi_node/HTTPRequestFactory.h"
#include "pi_node/PictureParseHTTPRequestHandler.h"

namespace PiNode {

const std::string HTTPRequestFactory::INVALID_USER_AGENT = "Invalid user";

HTTPRequestFactory::HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown,
                                       CTSNCommon::GPIOControllerInterface &gpio,
                                       CTSNCommon::NodeContainerInterface *nodes,
                                       Common::EventExecutorInterface *eventExecutor,
                                       Common::EventExecutorInterface *cvExecutor,
                                       CTSNCommon::UartInterface *uart) :
    m_settings(CTSNCommon::Settings::getInstance()),
    m_shutdown(shutdown),
    m_gpio(gpio),
    m_nodes(nodes),
    m_eventExecutor(eventExecutor),
    m_cvExecutor(cvExecutor),
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
    else if (userAgent != m_settings.getSetting("NODE_AGENT")) {
        return new CTSNCommon::BadClientHTTPRequestHandler();
    }
    else if (request.getURI() == PICTURE_PARSE_URI) {
        return new PictureParseHTTPRequestHandler(m_cvExecutor, m_eventExecutor);
    }
    else if (request.getURI() == BATTERY_CHECK_URI) {
        return new BatteryCheckHTTPRequestHandler(m_nodes, m_eventExecutor, m_gpio, m_uart);
    }
    else if (request.getURI() == SHUTDOWN_URI) {
        return new CTSNCommon::ShutdownHTTPRequestHandler(m_shutdown);
    }
    else {
        return new CTSNCommon::NotFoundHTTPRequestHandler();
    }
}

}

