#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "EventExecutor.h"
#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/DataEvent.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

const std::string DataHTTPRequestHandler::GET_MESSAGE = "Usage:\nnode=x&type=y";
const std::string DataHTTPRequestHandler::POST_FAILURE_MISSING_FIELD = "Missing Field";
const std::string DataHTTPRequestHandler::POST_FAILURE_INVALID_NODE = "Node does not exist";
const std::string DataHTTPRequestHandler::POST_FAILURE_INVALID_RESULT_TYPE = "Result type does not exist";

const std::string NODE_FORM_DATA = "node";
const std::string RESULT_TYPE_FORM_DATA = "type";

DataHTTPRequestHandler::DataHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                               MariaDBInterface *mariadb) :
    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb)
{

}

DataHTTPRequestHandler::~DataHTTPRequestHandler() {

}

void DataHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, 
                                               Poco::Net::HTTPServerResponse &response) {
    
}

void DataHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                              Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

