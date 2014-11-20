#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "EventExecutorInterface.h"
#include "gateway/DatabasePokeEvent.h"
#include "gateway/DatabasePokeHTTPRequestHandler.h"
#include "gateway/MariaDBInterface.h"

namespace Gateway {

const std::string DatabasePokeHTTPRequestHandler::POST_TRUE_MESSAGE = "Poking Databse Connection...";
const std::string DatabasePokeHTTPRequestHandler::POST_FALSE_MESSAGE = "Not Pokeing Database Connection.";
const std::string DatabasePokeHTTPRequestHandler::GET_MESSAGE = "Usage: poke=true/false";
const std::string DatabasePokeHTTPRequestHandler::POKE_FORM_DATA = "poke";

DatabasePokeHTTPRequestHandler::DatabasePokeHTTPRequestHandler(MariaDBInterface *mariadb,
                                                               Common::EventExecutorInterface *eventExecutor) :
    m_mariadb(mariadb),
    m_eventExecutor(eventExecutor)
{
}

DatabasePokeHTTPRequestHandler::~DatabasePokeHTTPRequestHandler() {

}

void DatabasePokeHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, 
                                                       Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        if (form[POKE_FORM_DATA] == "true") {
            std::shared_ptr<DatabasePokeEvent> event(new DatabasePokeEvent(m_mariadb));
            m_eventExecutor->addEvent(event);
            sendSuccessResponse(response, POST_TRUE_MESSAGE);
        }
        else {
            sendSuccessResponse(response, POST_FALSE_MESSAGE);
        }
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response);
    }
}

void DatabasePokeHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, 
                      Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

