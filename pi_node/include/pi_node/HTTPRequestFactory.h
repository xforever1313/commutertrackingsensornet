#ifndef HTTP_REQUEST_FACTORY_H_
#define HTTP_REQUEST_FACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "ctsn_common/ShutdownInterface.h"
#include "EventExecutorInterface.h"
//#include "gateway/UartInterface.h"

namespace PiNode {

class HTTPRequestFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown,
                           //UartInterface *uart,
                           Common::EventExecutorInterface *eventExecutor);

        ~HTTPRequestFactory();

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

    private:
        static const std::string INVALID_USER_AGENT;

        HTTPRequestFactory() = delete;

        CTSNCommon::ShutdownInterface *m_shutdown;
        Common::EventExecutorInterface *m_eventExecutor;
        //UartInterface *m_uart;
};

}

#endif

