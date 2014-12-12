#ifndef XBEE_TX_HTTP_REQUEST_HANDLER_H_
#define XBEE_TX_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"
#include "gateway/NodeContainerInterface.h"
#include "ctsn_common/UartInterface.h"

namespace Gateway {

/**
 * \class XBeeTxHTTPRequestHandler
 * \brief Handles the http request when an agent wants the gateway to
 *        send a message out via XBee.
 * \note To use, do an http post request with the data in the following form:
 *       node=nodeNumberToSendTo&message=message to send.
 */
class XBeeTxHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        XBeeTxHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                 CTSNCommon::UartInterface *uart,
                                 NodeContainerInterface *nodes);
        ~XBeeTxHTTPRequestHandler();

        void handleGetRequest(Poco::Net::HTTPServerRequest &request,
                              Poco::Net::HTTPServerResponse &response) override;

        void handlePostRequest(Poco::Net::HTTPServerRequest &request,
                               Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_MESSAGE;
        static const std::string POST_MISSING_FIELD_MESSAGE;
        static const std::string GET_MESSAGE;
        static const std::string MESSAGE_FORM_DATA;
        static const std::string NODE_FORM_DATA;

        XBeeTxHTTPRequestHandler() = delete;
        XBeeTxHTTPRequestHandler(const XBeeTxHTTPRequestHandler&);

        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::UartInterface *m_uart;
        NodeContainerInterface *m_nodes;
};

}

#endif

