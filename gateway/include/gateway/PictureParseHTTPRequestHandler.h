#ifndef PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_
#define PICTURE_PARSE_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "EventExecutorInterface.h"

namespace Gateway {

class PictureParseHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        PictureParseHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                       CTSNCommon::HTTPPosterInterface *httpPoster);

        ~PictureParseHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_SUCCESS;
        static const std::string POST_MISSING_FIELD;
        static const std::string GET_MESSAGE;
        static const std::string NODE_FORM_DATA;
        static const std::string PART_FORM_DATA;
        static const std::string DATA_FORM_DATA;

        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::HTTPPosterInterface *m_httpPoster;
};

}

#endif

