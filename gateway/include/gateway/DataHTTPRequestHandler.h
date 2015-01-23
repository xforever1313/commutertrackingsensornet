#ifndef DATA_HTTP_REQUEST_HANDLER_H_
#define DATA_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "gateway/DataEvent.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

/**
 * \class DataHTTPRequestHandler
 * \brief Handles the http request when an agent wants to tell the database
 *        an entity was discovered on the trail.
 */
class DataHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        DataHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                               MariaDBInterface *mariadb,
                               CTSNCommon::NodeContainerInterface *nodes);

        ~DataHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_SUCCESS_MESSAGE;
        static const std::string POST_FAILURE_MISSING_FIELD;
        static const std::string POST_FAILURE_INVALID_NODE;
        static const std::string POST_FAILURE_INVALID_RESULT_TYPE;

        static const std::string NODE_FORM_DATA;
        static const std::string RESULT_TYPE_FORM_DATA;

        DataHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
        CTSNCommon::NodeContainerInterface *m_nodes;
};

}

#endif

