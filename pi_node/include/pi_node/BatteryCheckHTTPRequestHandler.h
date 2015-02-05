#ifndef BATTERY_CHECK_HTTP_REQUEST_HANDLER_H_
#define BATTERY_CHECK_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/GPIOControllerInterface.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/UartInterface.h"
#include "EventExecutorInterface.h"

namespace PiNode {

class BatteryCheckHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        BatteryCheckHTTPRequestHandler(CTSNCommon::NodeContainerInterface *nodes,
                                       Common::EventExecutorInterface *eventExecutor,
                                       CTSNCommon::GPIOControllerInterface &gpio,
                                       CTSNCommon::UartInterface *uart);

        ~BatteryCheckHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_SUCCESS;
        static const std::string POST_FALSE_MESSAGE;
        static const std::string CHECK_FORM_DATA;

        BatteryCheckHTTPRequestHandler() = delete;


        CTSNCommon::NodeContainerInterface *m_nodes;
        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::GPIOControllerInterface &m_gpio;
        CTSNCommon::UartInterface *m_uart;
};

}

#endif
