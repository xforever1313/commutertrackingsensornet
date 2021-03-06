#ifndef NODE_STATUS_UPDATE_HTTP_REQUEST_HANDLER_H_
#define NODE_STATUS_UPDATE_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeStatusUpdateEvent.h"

namespace Gateway {

/**
 * \class NodeStatusUpdateHTTPRequestHandler
 * \brief Handles the http request when a trail node wants the
          gateway to update its status.
 * \note To use, do an http post request with the data in the following form:
 *       node=nodeNumber&status=statusNumber
 */
class NodeStatusUpdateHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        NodeStatusUpdateHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                           CTSNCommon::NodeContainerInterface *nodes,
                                           MariaDBInterface *mariadb);

        ~NodeStatusUpdateHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;

    private:
        NodeStatusUpdateHTTPRequestHandler() = delete;

        static const std::string GET_MESSAGE;
        static const std::string POST_SUCCESS_MESSAGE;
        static const std::string POST_MISSING_FIELD_MESSAGE;
        static const std::string NODE_FORM_DATA;
        static const std::string STATUS_FORM_DATA;

        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::NodeContainerInterface *m_nodes;
        MariaDBInterface *m_mariadb;
};

}

#endif

