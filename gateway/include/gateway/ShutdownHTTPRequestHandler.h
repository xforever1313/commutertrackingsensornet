#ifndef SHUTDOWNHTTPREQUESTHANDLER_H_
#define SHUTDOWNHTTPREQUESTHANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/ShutdownInterface.h"

namespace Gateway {

class ShutdownHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        ShutdownHTTPRequestHandler(ShutdownInterface *shutdown);
        ~ShutdownHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_MESSAGE;
        static const std::string GET_MESSAGE;

        ShutdownHTTPRequestHandler() = delete;

        ShutdownInterface *m_shutdown;
};

}

#endif
