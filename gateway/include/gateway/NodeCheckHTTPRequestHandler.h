#ifndef NODE_CHECK_HTTP_REQUEST_HANDLER_H_
#define NODE_CHECK_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeContainerInterface.h"

namespace Gateway {

class NodeCheckHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        NodeCheckHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                    MariaDBInterface *mariadb,
                                    NodeContainerInterface *nodes);

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
        NodeContainerInterface *m_nodes;
};

}

#endif

