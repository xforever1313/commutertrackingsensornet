#include <memory>
#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>
#include <vector>

#include "gateway/BaseHTTPRequestHandler.h"
#include "gateway/TextMessageEvent.h"
#include "gateway/TextMessageHTTPRequestHandler.h"
#include "StringOps.h"

namespace Gateway {

const std::string TextMessageHTTPRequestHandler::GET_MESSAGE = "Usage:\nnumbers=xxxxxxxxxx,yyyyyyyyyy,zzzzzzzzzz&providers=1,2,3&Subject=mySubject&message=some message";
const std::string TextMessageHTTPRequestHandler::POST_FAILURE_NON_MATCHING_NUMBERS_AND_PROVIDERS_MESSAGE = "Number of numbers does not match the number of providers given.";
const std::string TextMessageHTTPRequestHandler::POST_FAILURE_INVALID_PROVIDER_MESSAGE = "Invalid provider given";
const std::string TextMessageHTTPRequestHandler::POST_FAILURE_MISSING_FIELD_MESSAGE = "Missing field.";
const std::string TextMessageHTTPRequestHandler::POST_SUCCESS_MESSAGE = "Text message sent.";
const std::string TextMessageHTTPRequestHandler::NUMBERS_FORM_DATA = "numbers";
const std::string TextMessageHTTPRequestHandler::PROVIDERS_FORM_DATA = "providers";
const std::string TextMessageHTTPRequestHandler::SUBJECT_FORM_DATA = "subject";
const std::string TextMessageHTTPRequestHandler::MESSAGE_FORM_DATA = "message";

TextMessageHTTPRequestHandler::TextMessageHTTPRequestHandler(Common::EventExecutorInterface *eventExecutor) :
    m_eventExecutor(eventExecutor)
{

}

TextMessageHTTPRequestHandler::~TextMessageHTTPRequestHandler() {

}

void TextMessageHTTPRequestHandler::handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        Poco::Net::HTMLForm form(request, request.stream());
        std::vector<std::string> numbers = Common::StringOps::split(form[NUMBERS_FORM_DATA], ',');
        std::vector<std::string> providerStrings = Common::StringOps::split(form[PROVIDERS_FORM_DATA], ',');
        const std::string &subject = form[SUBJECT_FORM_DATA];
        const std::string &message = form[MESSAGE_FORM_DATA];

        std::vector <TextMessageEvent::Provider> providers;

        // Ensure the number of numbers and providers match
        if (numbers.size() != providerStrings.size()) {
            sendBadRequestResponse(response, POST_FAILURE_NON_MATCHING_NUMBERS_AND_PROVIDERS_MESSAGE);
        }
        // Ensure all providers given are valid
        else if (!convertProviderStringsToEnums(providerStrings, providers)){
            sendBadRequestResponse(response, POST_FAILURE_INVALID_PROVIDER_MESSAGE);
        }
        // We are good, create event
        else {
            std::map<std::string, TextMessageEvent::Provider> numberMap;
            for (size_t i = 0, size = numbers.size(); i < size; ++i) {
                numberMap[numbers[i]] = providers[i];
            }
            std::shared_ptr<TextMessageEvent> event (new TextMessageEvent(numberMap, subject, message));
            m_eventExecutor->addEvent(event);

            sendSuccessResponse(response, POST_SUCCESS_MESSAGE);
        }
    }
    catch (const Poco::NotFoundException &e) {
        sendBadRequestResponse(response, POST_FAILURE_MISSING_FIELD_MESSAGE);
    }

}

void TextMessageHTTPRequestHandler::handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    sendSuccessResponse(response, GET_MESSAGE);
}

bool TextMessageHTTPRequestHandler::convertProviderStringsToEnums(const std::vector<std::string> &rawStrings, std::vector<TextMessageEvent::Provider> &providers) {
    bool ret = true;

    for (size_t i = 0, size = rawStrings.size(); (i < size) && ret; ++i) {
        // Ensure each string is one character and only one character long
        if (rawStrings[i].size() != 1) {
            ret = false;
        }
        // Ensure its not out of range.
        else if ((rawStrings[i][0] <= '0') || rawStrings[i][0] > '9') {
            ret = false;
        }
        else {
            providers.push_back(static_cast<TextMessageEvent::Provider>(rawStrings[i][0] - '0'));
        }
    }

    return ret;
}

}
