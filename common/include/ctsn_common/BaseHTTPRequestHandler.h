#ifndef BASEHTTPREQUESTHANDLER_H
#define BASEHTTPREQUESTHANDLER_H

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

namespace CTSNCommon {

/**
 * \class BaseHTTPRequestHandler
 * \brief All http request handlers inherit from this.  They must implement
 *        handlePostRequest and handleGetRequest.
 */
class BaseHTTPRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        /**
         * \brief overrides the required method from parent class.  This will call handleGetRequest or handlePostRequest respectfully
         */
        void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    protected:
        BaseHTTPRequestHandler();
        virtual ~BaseHTTPRequestHandler();

        /**
         * \brief Called when a post request comes through
         */
        virtual void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) = 0;

        /**
         * \brief Called when a get request comes through
         */
        virtual void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) = 0;

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
