#ifndef TEXT_MESSAGE_EVENT_H_
#define TEXT_MESSAGE_EVENT_H_

#include <map>

#include "EventInterface.h"
#include "gateway/EmailerInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class TextMessageEvent
 * \brief This event sents a text message to the provided numbers.
 */
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
            UNKNOWN = 10
        };

        static const std::map<Provider, std::string> PROVIDER_NAMES;

        /**
         * \throw std::invalid_argument if inputted string is not a provider.
         */
        static Provider convertStringToProvider(const std::string &s);

        /**
         * \param numbers The Keys are the phone number in the form of 1112223333.  This is in the form
         *                of a map so a number only appears once, and is tied to a provider.
         * \param subject The subject of the message to be sent.
         * \param message The message itself (the body).
         */
        TextMessageEvent(const std::map<std::string, Provider> &numbers,
                         const std::string &subject,
                         const std::string &message,
                         Common::IO::LoggerBase &outLogger = Common::IO::ConsoleLogger::out,
                         Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~TextMessageEvent();

        void execute() override;

    private:
        /**
         * \brief Converts a string like (555)-555-2243 to 5555552243.
         *        Spaces and symbols are stripped from the string
         * \throw std::invalid_argument if a letter is detected.
         */
        static std::string convertPhoneStringToNumber(const std::string &number);

        static const std::string BAD_NUMBER_ERROR;
        static const std::map <Provider, std::string> PROVIDER_EMAIL;

        TextMessageEvent() = delete;
        TextMessageEvent(const TextMessageEvent&) = delete;

        EmailerInterface *m_emailer;
        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

