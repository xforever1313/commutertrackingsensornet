#ifndef BASEHTTPREQUESTHANDLER_H
#define BASEHTTPREQUESTHANDLER_H

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

namespace Gateway {

class BaseHTTPRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        BaseHTTPRequestHandler();
        virtual ~BaseHTTPRequestHandler();

        /**
         * \brief Sends back message with status 200.
         */
        void sendSuccessResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "ACK");

        /**
         * \brief Sends back not found message with status 404
         */
        void sendNotFoundResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "Not Found");

        /**
         * \brief Sends back a bad request response, status 400
         */
        void sendBadRequestResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "Bad Request");

        /**
         * \brief Sends back a forbidden request response, status 403
         */
         void sendForbiddenResponse(Poco::Net::HTTPServerResponse &response, const std::string &message = "Forbidden");

    private:
        void sendResponse(Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse::HTTPStatus status, const std::string &message);
};

}

#endif // BASEHTTPREQUESTHANDLER_H
