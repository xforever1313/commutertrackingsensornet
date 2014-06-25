#ifndef EMAIL_EVENT_H_
#define EMAIL_EVENT_H_

#include <map>

#include "EventInterface.h"
#include "gateway/EmailerInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class EmailEvent : public Common::EventInterface {
    public:
        EmailEvent(const std::map <std::string, std::string> &addresses,
                   const std::string &subject, const std::string &message,
                   Common::IO::LoggerBase &outLogger = Common::IO::ConsoleLogger::out,
                   Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~EmailEvent();

        void execute() override;

    private:
        EmailEvent() = delete;
        EmailEvent(const EmailEvent&) = delete;

        EmailerInterface *m_emailer;
        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif
