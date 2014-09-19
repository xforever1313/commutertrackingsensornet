#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "EventInterface.h"
#include "gateway/EmailEvent.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBWrapper.h"
#include "gateway/LogEvent.h"
#include "gateway/TextMessageEvent.h"
#include "io/LoggerBase.h"

namespace Gateway {

const std::string ErrorEvent::INVALID_ERROR_NUMBER = "Invalid error number passed in.";
const std::string ErrorEvent::MISMATCHED_COLUMNS = "MariaDB error: Mismatched Columns";
const std::string ErrorEvent::SUBJECT = "CTSN ERROR";

ErrorEvent::ErrorEvent(ErrorNumber error, unsigned int node, MariaDBInterface *mariadb,
                       Common::IO::LoggerBase &outLogger /*= Common::IO::ConsoleLogger::out */, 
                       Common::IO::LoggerBase &errLogger /*= Common::IO::ConsoleLogger::err */) :
    m_errorNumber(error),
    m_node(node),
    m_mariadb(mariadb),
    m_outLogger(outLogger),
    m_errLogger(errLogger),
    m_emailEvent(nullptr),
    m_logEvent(nullptr),
    m_textMessageEvent(nullptr),
    m_errorMessageResult(new MariaDBWrapper::MariaDBResult(m_mariadb)),
    m_userResult(new MariaDBWrapper::MariaDBResult(m_mariadb)),
    m_errorMessage(std::string(""))
{

}

ErrorEvent::~ErrorEvent() {
    delete m_emailEvent;
    delete m_logEvent;
    delete m_textMessageEvent;
    delete m_userResult;
    delete m_errorMessageResult;
}

void ErrorEvent::queryErrorMessage() {
    m_mariadb->mysql_real_query("SELECT * FROM error_messages WHERE id=" + std::to_string(m_errorNumber) + ";");

    m_errorMessageResult->storeResult();
    std::vector<std::string> messages = m_errorMessageResult->getValuesFromColumn("MESSAGES");

    // If no messages return, bad error number given.
    if (messages.size() == 0) {
        throw std::out_of_range(INVALID_ERROR_NUMBER);
    }

    m_errorMessage = std::to_string(m_node) + ": " + messages[0];
}

void ErrorEvent::queryForStaffedUsers() {
    // Get the staffed users
    m_mariadb->mysql_real_query("SELECT * FROM ctsn_user JOIN auth_user ON ctsn_user.user_id=auth_user.id WHERE is_staff=1;");

    // Store the last query.
    m_userResult->storeResult();
}

void ErrorEvent::setupEmailEvent() {
    std::vector<std::string> emails = m_userResult->getValuesFromColumn("email");
    std::vector<std::string> firstName = m_userResult->getValuesFromColumn("first_name");
    std::vector<std::string> lastName = m_userResult->getValuesFromColumn("last_name");

    if ((emails.size() != firstName.size()) || (emails.size() != lastName.size())) {
        throw std::runtime_error(MISMATCHED_COLUMNS);
    }

    std::map<std::string, std::string> addresses;
    for (size_t i = 0; i < emails.size(); ++i) {
        if (emails[i] != "") {
            addresses[emails[i]] = firstName[i] + " " + lastName[i];
        }
    }
    m_emailEvent = new EmailEvent(addresses, SUBJECT, m_errorMessage, m_outLogger, m_errLogger);
}

void ErrorEvent::setupLogEvent() {
    m_logEvent = new LogEvent(m_errorNumber, m_node, m_mariadb, m_errLogger);
}

void ErrorEvent::setupTextEvent() {
    std::vector<std::string> numbers = m_userResult->getValuesFromColumn("PHONE_NUMBER");
    std::vector<std::string> providers = m_userResult->getValuesFromColumn("PROVIDER");

    if (numbers.size() != providers.size()) {
        throw std::runtime_error(MISMATCHED_COLUMNS);
    }

    std::map<std::string, TextMessageEvent::Provider> contacts;
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] != "") {
            try {
                contacts[numbers[i]] = TextMessageEvent::convertStringToProvider(providers[i]);
            }
            catch (const std::invalid_argument &e) {
                m_errLogger.writeLine(e.what());
            }
        }
    }

    m_textMessageEvent = new TextMessageEvent(contacts, SUBJECT, m_errorMessage, m_outLogger, m_errLogger);
}

void ErrorEvent::executeEvents() {
    try {
        m_logEvent->execute();
    }
    catch (const std::exception &e) {
        m_errLogger.writeLine(e.what());
    }

    try {
        m_textMessageEvent->execute();
    }
    catch (const std::exception &e) {
        m_errLogger.writeLine(e.what());
    }

    try {
        m_emailEvent->execute();
    }
    catch (const std::exception &e) {
        m_errLogger.writeLine(e.what());
    }
}

void ErrorEvent::execute() {
    try {
        queryErrorMessage();
        queryForStaffedUsers();
        setupEmailEvent();
        setupLogEvent();
        setupTextEvent();
        executeEvents();
    }
    catch (const std::exception &e) {
        m_errLogger.writeLine(e.what());
    }
}

}

