#ifndef PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_
#define PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "EventExecutorInterface.h"
#include "ctsn_common/UartInterface.h"

namespace PiNode {

class PictureParseHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        PictureParseHTTPRequestHandler(CTSNCommon::NodeContainerInterface *nodes,
                                       CTSNCommon::UartInterface *uart,
                                       Common::EventExecutorInterface *cvExecutor,
                                       Common::EventExecutorInterface *eventExecutor);
        ~PictureParseHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_SUCCESS;
        static const std::string POST_FAILURE;
        static const std::string REMOVE_FORM_DATA;
        static const std::string PICTURE_FORM_DATA;

        PictureParseHTTPRequestHandler() = delete;

        CTSNCommon::NodeContainerInterface *m_nodes;
        CTSNCommon::UartInterface *m_uart;
        Common::EventExecutorInterface *m_cvExecutor;
        Common::EventExecutorInterface *m_eventExecutor;
};

}

#endif
