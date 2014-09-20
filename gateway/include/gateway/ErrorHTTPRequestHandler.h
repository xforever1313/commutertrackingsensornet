#ifndef ERROR_HTTP_REQUEST_HANDLER_H_
#define ERROR_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutor.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

class ErrorHTTPRequestHandler : public BaseHTTPRequestHandler {

    public:
        ErrorHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor, 
                                MariaDBInterface *mariadbi);

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
};

}

#endif

