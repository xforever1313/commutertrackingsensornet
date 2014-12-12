#ifndef LOG_EVENT_H_
#define LOG_EVENT_H_

#include "EventInterface.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBInterface.h"
#include "ctsn_common/Node.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class LogEvent
 * \brief Call this class when an error needs to be logged to the database.
 * \note This class only logs to the database, and does not send out emails or texts.
 */
class LogEvent : public Common::EventInterface {

    public:
        /**
         * \brief Constructor
         * \param error The error number to log.
         * \param node The node that generated the error.
         */
        LogEvent(ErrorNumber error, const CTSNCommon::Node &node, MariaDBInterface *mariadb,
                 Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~LogEvent();

        void execute() override;

    private:
       LogEvent() = delete;
       LogEvent(const LogEvent &) = delete;

       ErrorNumber m_errorNumber;
       const CTSNCommon::Node m_node;
       MariaDBInterface *m_mariadb;
       Common::IO::LoggerBase &m_errLogger;
};

}

#endif

