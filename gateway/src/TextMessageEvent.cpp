#include <cctype>
#include <stdexcept>
#include <string>
#include <map>

#include "gateway/Emailer.h"
#include "gateway/TextMessageEvent.h"

namespace Gateway {

const std::string TextMessageEvent::BAD_NUMBER_ERROR = "Invalid phone number:";

const std::map<TextMessageEvent::Provider, std::string> TextMessageEvent::PROVIDER_NAMES = {
        { ATT, "AT&T" },
        { VERIZON, "Verizon" },
        { TMOBILE, "T-mobile" },
        { SPRINT, "Sprint" },
        { VIRGIN_MOBILE, "Virgin Mobile" },
        { US_CELLULAR, "US Cellulat" },
        { NEXTEL, "Nextel" },
        { BOOST, "Boost Mobile" },
        { ALLTEL, "Alltel" },
        { UNKNOWN, "Unknown" }
};

const std::map<TextMessageEvent::Provider, std::string> TextMessageEvent::PROVIDER_EMAIL = {
        { ATT, "txt.att.net" },
        { VERIZON, "vtext.com" },
        { TMOBILE, "tmomail.net" },
        { SPRINT, "messaging.sprintpcs.com" },
        { VIRGIN_MOBILE, "vmobl.com" },
        { US_CELLULAR, "email.uscc.net" },
        { NEXTEL, "messaging.nextel.com" },
        { BOOST, "myboostmobile.com" },
        { ALLTEL, "message.alltel.com" },
        { UNKNOWN, "Unknown" }
};

TextMessageEvent::TextMessageEvent(const std::map <std::string, Provider> &numbers,
                                   const std::string &subject, const std::string &message,
                                   Common::IO::LoggerBase &outLogger /*= Common::IO::ConsoleLogger::out*/,
                                   Common::IO::LoggerBase &errLogger /*= Common::IO::ConsoleLogger::err*/) :
                                   m_emailer(nullptr),
                                   m_outLogger(outLogger),
                                   m_errLogger(errLogger)
{
    std::map <std::string, std::string> addresses;
    for (const auto &number : numbers) {
        try {
            std::string numberString = convertPhoneStringToNumber(number.first);
            addresses[numberString + '@' + PROVIDER_EMAIL.at(number.second) ] = numberString;
        }
        catch(const std::invalid_argument &e) {
            m_errLogger.writeLineWithTimeStamp(e.what());
        }
    }
    m_emailer = new Emailer(addresses, subject, message);
}

TextMessageEvent::~TextMessageEvent() {
    delete m_emailer;
}

void TextMessageEvent::execute() {
    try {
        std::string curlOutput = m_emailer->send();
        m_outLogger.writeLineWithTimeStamp(curlOutput);
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

TextMessageEvent::Provider TextMessageEvent::convertStringToProvider(const std::string &s) {
    std::string error = s + " is not a provider.";
    if (s.size() != 1) {
        throw std::invalid_argument(error);
    }
    unsigned int i = s[0] - '0';
    if (i == 0) {
        throw std::invalid_argument(error);
    }
    else if (i >= UNKNOWN) {
        throw std::invalid_argument(error);
    }

    return static_cast<Provider>(i);
}

std::string TextMessageEvent::convertPhoneStringToNumber(const std::string &number) {
    std::string s;
    for (size_t i = 0; i < number.size(); ++i) {
        if (::isdigit(number[i])) {
            s += number[i];
        }
        else if (::isalpha(number[i])) {
            throw std::invalid_argument(BAD_NUMBER_ERROR + " " + number);
        }
    }

    return s;
}

}

