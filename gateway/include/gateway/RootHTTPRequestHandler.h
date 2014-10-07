#ifndef ROOT_HTTP_REQUEST_HANDLER
#define ROOT_HTTP_REQUEST_HANDLER

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "gateway/BaseHTTPRequestHandler.h"

namespace Gateway {

class RootHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        RootHTTPRequestHandler();

        ~RootHTTPRequestHandler();

         void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
         void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        const static std::string RESPONSE;
};

}

#endif

