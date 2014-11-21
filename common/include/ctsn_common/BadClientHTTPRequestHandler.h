#ifndef BADCLIENTHTTPREQUESTHANDLER_H_
#define BADCLIENTHTTPREQUESTHANDLER_H_

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "ctsn_common/BaseHTTPRequestHandler.h"

namespace CTSNCommon {

class BadClientHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        BadClientHTTPRequestHandler();
        ~BadClientHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string MESSAGE;

        void handleHTTPRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

}

#endif
