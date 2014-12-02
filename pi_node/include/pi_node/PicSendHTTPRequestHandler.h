#ifndef PIC_SEND_HTTP
#define PIC_SEND_HTTP

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "ctsn_common/UartInterface.h"

namespace PiNode {

class PicSendHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        PicSendHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                  CTSNCommon::UartInterface *uart);

        ~PicSendHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::UartInterface *m_uart;
};

}

#endif

