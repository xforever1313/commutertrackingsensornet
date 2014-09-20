#ifndef LOG_MESSAGE_HTTP_REQUEST_HANDLER_H_
#define LOG_MESSAGE_HTTP_REQUEST_HANDLER_H_

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutorInterface.h"
#include "BaseHTTPRequestHandler.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/LogEvent.h"

namespace Gateway {

class LogMessageHTTPRequestHandler : public BaseHTTPRequestHandler {

    public:
        LogMessageHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                     MariaDBInterface *mariadb);

        ~LogMessageHTTPRequestHandler();

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;

    private:
        static const std::string GET_MESSAGE;
        static const std::string POST_FAILURE_MISSING_FIELD_MESSAGE;
        static const std::string POST_FAILURE_INVALID_NODE;
        static const std::string POST_FAILURE_INVALID_MESSAGE;
        static const std::string POST_SUCCESS_MESSAGE;
        static const std::string NODE_FORM_DATA;
        static const std::string MESSAGE_FORM_DATA;

        /**
         * \brief Converts the given string to the node number 
         * \throws std::out_of_range if the node is not a valid node number
         * \throws std::invalid_arugment if the given string is not an int
         */
        static unsigned int convertStringToNodeNumber(const std::string &nodeString); 

        /**
         * \brief Converts the given string to the message error number 
         * \throws std::out_of_range if the message is not a valid message number
         * \throws std::invalid_arugment if the given string is not an int
         */
        static ErrorNumber convertStringToMessage(const std::string &nodeString); 

        LogMessageHTTPRequestHandler() = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
};

}

#endif


