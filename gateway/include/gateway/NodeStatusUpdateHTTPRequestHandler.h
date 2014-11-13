#ifndef NODE_STATUS_UPDATE_HTTP_REQUEST_HANDLER_H_
#define NODE_STATUS_UPDATE_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/Node.h"
#include "gateway/NodeContainerInterface.h"
#include "gateway/NodeStatusUpdateEvent.h"

namespace Gateway {

class NodeStatusUpdateHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        NodeStatusUpdateHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                           NodeContainerInterface *nodes);

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
        NodeContainerInterface *m_nodes;
};

}

#endif

