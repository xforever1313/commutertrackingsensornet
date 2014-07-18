#ifndef BADCLIENTHTTPREQUESTHANDLER_H_
#define BADCLIENTHTTPREQUESTHANDLER_H_

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "gateway/BaseHTTPRequestHandler.h"

namespace Gateway {

class BadClientHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        BadClientHTTPRequestHandler();
        ~BadClientHTTPRequestHandler();

        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string MESSAGE;
};

}

#endif
