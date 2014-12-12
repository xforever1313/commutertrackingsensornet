#ifndef TEXTMESSAGEHTTPREQUESTHANDLER_H
#define TEXTMESSAGEHTTPREQUESTHANDLER_H

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "gateway/TextMessageEvent.h"

namespace Gateway {

/**
 * \class TextMessageHTTPRequestHandler
 * \brief Handles the http request when an agent wants the gateway to
 *        send an email out.
 * \note To use, do an http post request with the data in the following form:
 *       numbers=xxxxxxxxx,yyyyyyyyy&providers=1,2&subject=theSubject&message=the message to send.
 *       The address and the names must have the same amount of comma separated values in it.
 */
class TextMessageHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        TextMessageHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor);
        ~TextMessageHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_FAILURE_NON_MATCHING_NUMBERS_AND_PROVIDERS_MESSAGE;
        static const std::string POST_FAILURE_INVALID_PROVIDER_MESSAGE;
        static const std::string POST_FAILURE_MISSING_FIELD_MESSAGE;
        static const std::string POST_SUCCESS_MESSAGE;
        static const std::string NUMBERS_FORM_DATA;
        static const std::string PROVIDERS_FORM_DATA;
        static const std::string SUBJECT_FORM_DATA;
        static const std::string MESSAGE_FORM_DATA;

        TextMessageHTTPRequestHandler() = delete;

        /**
         * \brief converts the raw strings to enums.
         * \return true if conversion successful, otherwise something went wrong.
         */
        bool convertProviderStringsToEnums(const std::vector<std::string> &rawStrings, std::vector<TextMessageEvent::Provider> &providers);

        Common::EventExecutorInterface *m_eventExecutor;
};

}

#endif // TEXTMESSAGEHTTPREQUESTHANDLER_H
