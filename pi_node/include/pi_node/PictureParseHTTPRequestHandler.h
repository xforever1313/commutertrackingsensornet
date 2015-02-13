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
        PictureParseHTTPRequestHandler(Common::EventExecutorInterface *cvExecutor);
        ~PictureParseHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;

    private:
        PictureParseHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_cvExecutor;
};

}

#endif // PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_
