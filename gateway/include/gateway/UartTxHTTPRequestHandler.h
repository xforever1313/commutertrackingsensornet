#ifndef UARTTXHTTPREQUESTHANDLER_H
#define UARTTXHTTPREQUESTHANDLER_H

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "gateway/UartInterface.h"

namespace Gateway {

class UartTxHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        UartTxHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor, UartInterface *uart);
        ~UartTxHTTPRequestHandler();

        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_MESSAGE;
        static const std::string GET_MESSAGE;
        static const std::string MESSAGE_FORM_DATA;

        UartTxHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        UartInterface *m_uart;
};

}

#endif // UARTTXHTTPREQUESTHANDLER_H
