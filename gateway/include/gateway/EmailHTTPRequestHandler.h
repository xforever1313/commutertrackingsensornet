#ifndef EMAIL_HTTP_REQUEST_HANDLER_H_
#define EMAIL_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "gateway/EmailEvent.h"

namespace Gateway {

/**
 * \class EmailHTTPRequestHandler
 * \brief Handles the http request when an agent wants the gateway to
 *        send an email out.
 * \note To use, do an http post request with the data in the following form:
 *       addresses=xxx@xxx.com,yyy@yyy.com&names=person 1,person 2&subject=theSubject&message=the message to send.
 *       The address and the names must have the same amount of comma separated values in it.
 */
class EmailHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {

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

