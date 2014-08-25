#ifndef EMAIL_HTTP_REQUEST_HANDLER_H_
#define EMAIL_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/EmailEvent.h"

namespace Gateway {

class EmailHTTPRequestHandler : public BaseHTTPRequestHandler {

    public:
        EmailHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor);
        ~EmailHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_FAILURE_NON_MATCHING_ADDRESSES_AND_NAMES_MESSAGE;
        static const std::string POST_FAILURE_MISSING_FIELD_MESSAGE;
        static const std::string POST_SUCCESS_MESSAGE;
        static const std::string ADDRESS_FORM_DATA;
        static const std::string NAMES_FORM_DATA;
        static const std::string SUBJECT_FORM_DATA;
        static const std::string MESSAGE_FORM_DATA;

        EmailHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
}; 

}

#endif

