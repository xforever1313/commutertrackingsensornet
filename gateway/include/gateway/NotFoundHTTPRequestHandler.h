#ifndef NOTFOUNDHTTPREQUESTHANDLER_H_
#define NOTFOUNDHTTPREQUESTHANDLER_H_

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

#include "gateway/BaseHTTPRequestHandler.h"

namespace Gateway {

class NotFoundHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        NotFoundHTTPRequestHandler();
        ~NotFoundHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string MESSAGE;

        void handleHTTPRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

}

#endif
