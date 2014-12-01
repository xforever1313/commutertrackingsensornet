#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

#include "ctsn_common/DataResultTypes.h"
#include "EventExecutor.h"
#include "gateway/DataEvent.h"
#include "gateway/DataHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/NodeContainerInterface.h"

namespace Gateway {

const std::string DataHTTPRequestHandler::GET_MESSAGE = "Usage:\nnode=x&type=y";
const std::string DataHTTPRequestHandler::POST_SUCCESS_MESSAGE = "Result sent";
const std::string DataHTTPRequestHandler::POST_FAILURE_MISSING_FIELD = "Missing Field";
const std::string DataHTTPRequestHandler::POST_FAILURE_INVALID_NODE = "Node does not exist";
const std::string DataHTTPRequestHandler::POST_FAILURE_INVALID_RESULT_TYPE = "Result type does not exist";

const std::string DataHTTPRequestHandler::NODE_FORM_DATA = "node";
const std::string DataHTTPRequestHandler::RESULT_TYPE_FORM_DATA = "type";

DataHTTPRequestHandler::DataHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor,
                                               MariaDBInterface *mariadb,
                                               NodeContainerInterface *nodes) :
    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb),
    m_nodes(nodes)
{

}

DataHTTPRequestHandler::~DataHTTPRequestHandler() {

}

void DataHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, 
                                               Poco::Net::HTTPServerResponse &response) {

   try {
        Poco::Net::HTMLForm form(request, request.stream());
        const std::string &nodeStr = form[NODE_FORM_DATA];
        const std::string &typeStr = form[RESULT_TYPE_FORM_DATA];

        const CTSNCommon::Node node = m_nodes->convertStringToNode(nodeStr);
        const CTSNCommon::DataResultType type = CTSNCommon::convertStringToResultType(typeStr);

        std::shared_ptr<DataEvent> event (new DataEvent(node, type, m_mariadb));
        m_eventExecutor->addEvent(event);

        sendSuccessResponse(response, POST_SUCCESS_MESSAGE);
   }
   catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_FAILURE_MISSING_FIELD);
   }
   catch(const std::exception &e) {
        sendBadRequestResponse(response, e.what());
   }
}

void DataHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request,
                                              Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

