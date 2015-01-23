#ifndef NODE_CHECK_HTTP_REQUEST_HANDLER_H_
#define NODE_CHECK_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

/**
 * \class NodeCheckHTTPRequestHandler
 * \brief Handles the http request when an agent wants the gateway to
 *        log an error to the database..
 * \note To use, do an http post request with the data in the following form:
 *       check=true
 */
class NodeCheckHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        NodeCheckHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                    MariaDBInterface *mariadb,
                                    CTSNCommon::NodeContainerInterface *nodes);

        ~NodeCheckHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;
    private:
        static const std::string POST_TRUE_MESSAGE;
        static const std::string POST_FALSE_MESSAGE;
        static const std::string POST_MISSING_FIELD;
        static const std::string GET_MESSAGE;
        static const std::string CHECK_FORM_DATA;

        NodeCheckHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
        CTSNCommon::NodeContainerInterface *m_nodes;
};

}

#endif

