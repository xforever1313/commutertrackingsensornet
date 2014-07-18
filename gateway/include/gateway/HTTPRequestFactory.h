#ifndef HTTPREQUESTFACTORY_H_
#define HTTPREQUESTFACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "ShutdownInterface.h"

namespace Gateway {

class HTTPRequestFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        HTTPRequestFactory(ShutdownInterface *shutdown);
        ~HTTPRequestFactory();

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

    private:
        static const std::string INVALID_USER_AGENT;

        HTTPRequestFactory() = delete;

        ShutdownInterface *m_shutdown;
};

}

#endif // HTTPREQUESTFACTORY_H_H
