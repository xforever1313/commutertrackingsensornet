#ifndef HTTPREQUESTFACTORY_H_
#define HTTPREQUESTFACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/ShutdownInterface.h"
#include "gateway/UartInterface.h"

namespace Gateway {

class HTTPRequestFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        HTTPRequestFactory(ShutdownInterface *shutdown, Common::EventExecutorInterface *eventExecutor, UartInterface *uart);
        ~HTTPRequestFactory();

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

    private:
        static const std::string INVALID_USER_AGENT;

        HTTPRequestFactory() = delete;

        ShutdownInterface *m_shutdown;
        Common::EventExecutorInterface *m_eventExecutor;
        UartInterface *m_uart;
};

}

#endif // HTTPREQUESTFACTORY_H_H
