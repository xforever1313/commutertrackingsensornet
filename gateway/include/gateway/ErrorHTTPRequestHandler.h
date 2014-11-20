#ifndef ERROR_HTTP_REQUEST_HANDLER_H_
#define ERROR_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutor.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeContainerInterface.h"

namespace Gateway {

class ErrorHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {

    public:
        ErrorHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor, 
                                MariaDBInterface *mariadb,
                                NodeContainerInterface *nodes);

        ~ErrorHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_FAILURE_MISSING_FIELD_MESSAGE;
        static const std::string POST_SUCCESS_MESSAGE;
        static const std::string NODE_FORM_DATA;
        static const std::string MESSAGE_FORM_DATA;

        ErrorHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
        NodeContainerInterface *m_nodes;
};

}

#endif

