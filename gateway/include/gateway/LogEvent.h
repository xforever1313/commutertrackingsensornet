#ifndef LOG_EVENT_H_
#define LOG_EVENT_H_

#include "EventInterface.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class LogEvent : public Common::EventInterface {

    public:
        LogEvent(ErrorNumber error, unsigned int node, MariaDBInterface *mariadb,
                 Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~LogEvent();

        void execute() override;

    private:
       LogEvent() = delete;
       LogEvent(const LogEvent &) = delete;

       ErrorNumber m_errorNumber;
       unsigned int m_node;
       MariaDBInterface *m_mariadb;
       Common::IO::LoggerBase &m_errLogger;
};

}

#endif

