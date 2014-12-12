#ifndef ERROR_EVENT_H_
#define ERROR_EVENT_H_

#include <string>
#include <vector>

#include "EventInterface.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBInterface.h"
#include "ctsn_common/Node.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class ErrorEvent
 * \brief Executed when an error occurs and the database needs to be
 *        updated, along with emails and texts being sent out to admins
 */
class ErrorEvent : public Common::EventInterface {

    public:
        /**
         * \brief Constructor
         * \param error The error number that occurred
         * \param node The node that generated the error
         */
        ErrorEvent(ErrorNumber error, const CTSNCommon::Node &node, MariaDBInterface *mariadb,
                   Common::IO::LoggerBase &outLogger = Common::IO::ConsoleLogger::out,
                   Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~ErrorEvent();

        /** \note Execute is not specifically unit tested as we don't want to
         *        actually send emails or texts.  Instead, each method
         *        execute calls is tested separetly
         */
        void execute() override;

    private:
        const static std::string INVALID_ERROR_NUMBER;
        const static std::string MISMATCHED_COLUMNS;
        const static std::string SUBJECT;

        ErrorEvent() = delete;
        ErrorEvent(const ErrorEvent &) = delete;

        /**
         * \throws std::runtime_error if query failes
         * \throws std::out_of_range if error number does not exist
         */
        void queryErrorMessage();

        /**
         * \throws std::runtime_error if query fails
         */
        void queryForStaffedUsers();

        void setupEmailEvent();
        void setupLogEvent();
        void setupTextEvent();
        void executeEvents();

        ErrorNumber m_errorNumber;
        const CTSNCommon::Node m_node;
        MariaDBInterface *m_mariadb;
        Common::IO::LoggerBase &m_outLogger;
        Common::IO::LoggerBase &m_errLogger;

        Common::EventInterface *m_emailEvent;
        Common::EventInterface *m_logEvent;
        Common::EventInterface *m_textMessageEvent;
        MariaDBResultInterface *m_errorMessageResult;
        MariaDBResultInterface *m_userResult;
        std::string m_errorMessage;
};

}

#endif

