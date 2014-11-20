#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>
#include <vector>

#include "gateway/EmailEvent.h"
#include "gateway/EmailHTTPRequestHandler.h"
#include "StringOps.h"

namespace Gateway {

const std::string EmailHTTPRequestHandler::GET_MESSAGE = "Usage:\naddresses=xxxxxxxxxx,yyyyyyyyyy,zzzzzzzzzz&names=derp,herp,foo bar&subject=mySubject&message=some message";
const std::string EmailHTTPRequestHandler::POST_FAILURE_NON_MATCHING_ADDRESSES_AND_NAMES_MESSAGE = "Number of adresses does not match the number of addresses given.";
const std::string EmailHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE = "Missing field.";
const std::string EmailHTTPRequestHandler::POST_SUCCESS_MESSAGE = "Text message sent.";
const std::string EmailHTTPRequestHandler::ADDRESS_FORM_DATA = "addresses";
const std::string EmailHTTPRequestHandler::NAMES_FORM_DATA = "names";
const std::string EmailHTTPRequestHandler::SUBJECT_FORM_DATA = "subject";
const std::string EmailHTTPRequestHandler::MESSAGE_FORM_DATA = "message";

EmailHTTPRequestHandler::EmailHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor) :
    m_eventExecutor(eventExecutor)
{

}

EmailHTTPRequestHandler::~EmailHTTPRequestHandler() {

}

void EmailHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        std::vector<std::string> addresses = Common::StringOps::split(form[ADDRESS_FORM_DATA], ',');
        std::vector<std::string> names = Common::StringOps::split(form[NAMES_FORM_DATA], ',');
        const std::string &subject = form[SUBJECT_FORM_DATA];
        const std::string &message = form[MESSAGE_FORM_DATA];

        // Ensure the number of numbers and providers match
        if (addresses.size() != names.size()) {
            sendBadRequestResponse(response, POST_FAILURE_NON_MATCHING_ADDRESSES_AND_NAMES_MESSAGE);
        }
        else {
            std::map<std::string, std::string> addressMap;
            for (size_t i = 0, size = names.size(); i < size; ++i) {
                addressMap[addresses[i]] = names[i];
            }
            std::shared_ptr<EmailEvent> event (new EmailEvent(addressMap, subject, message));
            m_eventExecutor->addEvent(event);

            sendSuccessResponse(response, POST_SUCCESS_MESSAGE);
        }
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_FAILURE_MISSING_FIELD_MESSAGE);
    }

}

void EmailHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

}

