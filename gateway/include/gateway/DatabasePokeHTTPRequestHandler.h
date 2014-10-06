#ifndef DATABASE_POKE_HTTP_REQUEST_HANDLER_H_
#define DATABASE_POKE_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/DatabasePokeEvent.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

class DatabasePokeHTTPRequestHandler : public BaseHTTPRequestHandler {

    public:
        DatabasePokeHTTPRequestHandler(MariaDBInterface *mariadb,
                                       Common::EventExecutorInterface *eventExecutor);

        ~DatabasePokeHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string POST_TRUE_MESSAGE;
        static const std::string POST_FALSE_MESSAGE;
        static const std::string GET_MESSAGE;
        static const std::string POKE_FORM_DATA;

        DatabasePokeHTTPRequestHandler() = delete;

        MariaDBInterface *m_mariadb;
        Common::EventExecutorInterface *m_eventExecutor;
};

}

#endif

