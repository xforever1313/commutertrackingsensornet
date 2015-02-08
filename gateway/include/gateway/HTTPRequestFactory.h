#ifndef HTTPREQUESTFACTORY_H_
#define HTTPREQUESTFACTORY_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <string>

#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/ShutdownInterface.h"
#include "ctsn_common/UartInterface.h"
#include "EventExecutorInterface.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

/**
 * \class HTTPRequestFactory
 * \brief Sends to the Poco Library handlers for whatever URI an agent
 *        requests.
 */
class HTTPRequestFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        HTTPRequestFactory(CTSNCommon::ShutdownInterface *shutdown,
                           Common::EventExecutorInterface *eventExecutor,
                           CTSNCommon::UartInterface *uart,
                           MariaDBInterface *mariadb,
                           CTSNCommon::NodeContainerInterface *nodes,
                           CTSNCommon::HTTPPosterInterface *httpPoster);
        ~HTTPRequestFactory();

        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;

    private:
        static const std::string INVALID_USER_AGENT;

        HTTPRequestFactory() = delete;

        CTSNCommon::ShutdownInterface *m_shutdown;
        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::UartInterface *m_uart;
        MariaDBInterface *m_mariadb;
        CTSNCommon::NodeContainerInterface *m_nodes;
        CTSNCommon::HTTPPosterInterface *m_httpPoster;
        CTSNCommon::Settings &m_settings;
};

}

#endif // HTTPREQUESTFACTORY_H_H
