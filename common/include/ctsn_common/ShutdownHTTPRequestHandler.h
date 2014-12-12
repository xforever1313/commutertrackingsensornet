#ifndef SHUTDOWNHTTPREQUESTHANDLER_H_
#define SHUTDOWNHTTPREQUESTHANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "ctsn_common/ShutdownInterface.h"
#include "ctsn_common/BaseHTTPRequestHandler.h"

namespace CTSNCommon {

/**
 * \class ShutdownHTTPRequestHandler
 * \brief When posted to, this handler calls shutdown on the passed in ShutdownInterface.
 * \note To use, do an http post request with the data in the following form:
 *       shutdown=true
 */
class ShutdownHTTPRequestHandler : public CTSNCommon::BaseHTTPRequestHandler {
    public:
        ShutdownHTTPRequestHandler(CTSNCommon::ShutdownInterface *shutdown);
        ~ShutdownHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_TRUE_MESSAGE;
        static const std::string POST_FALSE_MESSAGE;
        static const std::string GET_MESSAGE;
        static const std::string SHUTDOWN_FORM_DATA;

        ShutdownHTTPRequestHandler() = delete;

        CTSNCommon::ShutdownInterface *m_shutdown;
};

}

#endif
