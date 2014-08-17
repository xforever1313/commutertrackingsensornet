#ifndef TEXT_MESSAGE_EVENT_H_
#define TEXT_MESSAGE_EVENT_H_

#include <map>

#include "EventInterface.h"
#include "gateway/EmailerInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class TextMessageEvent : public Common::EventInterface {
    public:
        enum Provider {
            ATT = 1,
            VERIZON = 2,
            TMOBILE = 3,
            SPRINT = 4,
            VIRGIN_MOBILE = 5,
            US_CELLULAR = 6,
            NEXTEL = 7,
            BOOST = 8,
            ALLTEL = 9,
            UNKNOWN
        };

        static const std::map<Provider, std::string> PROVIDER_NAMES;

        /**
         * \param numbers The Keys are the phone number in the form of 1112223333
         */
        TextMessageEvent(const std::map<std::string, Provider> &numbers,
                         const std::string &subject,
                         const std::string &message,
                         Common::IO::LoggerBase &outLogger = Common::IO::ConsoleLogger::out,
                         Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~TextMessageEvent();

        void execute() override;

    private:
        static const std::map <Provider, std::string> PROVIDER_EMAIL;

        TextMessageEvent() = delete;
        TextMessageEvent(const TextMessageEvent&) = delete;

        EmailerInterface *m_emailer;
        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif
