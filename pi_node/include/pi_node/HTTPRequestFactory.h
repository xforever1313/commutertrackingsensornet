#ifndef HTTP_REQUEST_FACTORY_H_
#define HTTP_REQUEST_FACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "ctsn_common/GPIOControllerInterface.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/ShutdownInterface.h"
#include "EventExecutorInterface.h"
#include "ctsn_common/UartInterface.h"

namespace PiNode {

class HTTPRequestFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown,
                           CTSNCommon::GPIOControllerInterface &gpio,
                           CTSNCommon::NodeContainerInterface *nodes,
                           Common::EventExecutorInterface *eventExecutor,
                           CTSNCommon::UartInterface *uart);

        ~HTTPRequestFactory();

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

    private:
        static const std::string INVALID_USER_AGENT;

        HTTPRequestFactory() = delete;

        CTSNCommon::Settings &m_settings;
        CTSNCommon::ShutdownInterface *m_shutdown;
        CTSNCommon::GPIOControllerInterface &m_gpio;
        CTSNCommon::NodeContainerInterface *m_nodes;
        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::UartInterface *m_uart;
};

}

#endif

