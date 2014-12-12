#ifndef UARTTXHTTPREQUESTHANDLER_H
#define UARTTXHTTPREQUESTHANDLER_H

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/UartInterface.h"

namespace Gateway {

/**
 * \class UartTxHTTPRequestHandler
 * \brief Handles the http request when an agent wants the gateway to
 *        send a message out over uart with no preprocessing.
 * \note To use, do an http post request with the data in the following form:
 *       message=the message you want to send
 */
class UartTxHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        UartTxHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                 CTSNCommon::UartInterface *uart);
        ~UartTxHTTPRequestHandler();

        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_MESSAGE;
        static const std::string GET_MESSAGE;
        static const std::string MESSAGE_FORM_DATA;

        UartTxHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::UartInterface *m_uart;
};

}

#endif // UARTTXHTTPREQUESTHANDLER_H
