#ifndef DATA_HTTP_REQUEST_HANDLER_H_
#define DATA_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/DataEvent.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

class DataHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        DataHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                               MariaDBInterface *mariadb);

        ~DataHTTPRequestHandler();
        
        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_FAILURE_MISSING_FIELD;
        static const std::string POST_FAILURE_INVALID_NODE;
        static const std::string POST_FAILURE_INVALID_RESULT_TYPE;

        static const std::string NODE_FORM_DATA;
        static const std::string RESULT_TYPE_FORM_DATA;

        DataHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
};

}

#endif

