#include <stdexcept>
#include <string>
#include <map>

#include "gateway/Emailer.h"
#include "gateway/TextMessageEvent.h"

namespace Gateway {

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
        addresses[number.first + '@' + PROVIDER_EMAIL.at(number.second) ] = number.first;
    }
    m_emailer = new Emailer(addresses, subject, message);
}

TextMessageEvent::~TextMessageEvent() {
    delete m_emailer;
}

void TextMessageEvent::execute() {
    try {
        std::string curlOutput = m_emailer->send();
        m_outLogger.writeLine(curlOutput);
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLine(e.what());
    }
}

}