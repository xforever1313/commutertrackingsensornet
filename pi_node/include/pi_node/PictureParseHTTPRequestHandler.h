#ifndef PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_
#define PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "EventExecutorInterface.h"

namespace PiNode {

class PictureParseHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        PictureParseHTTPRequestHandler(Common::EventExecutorInterface *cvExecutor,
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

        Common::EventExecutorInterface *m_cvExecutor;
        Common::EventExecutorInterface *m_eventExecutor;
};

}

#endif
