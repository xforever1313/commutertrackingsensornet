#include <stdexcept>
#include <string>

#include "gateway/ErrorNumbers.h"
#include "gateway/LogEvent.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/Node.h"
#include "io/LoggerBase.h"

namespace Gateway {

LogEvent::LogEvent(ErrorNumber error, const Node &node, MariaDBInterface *mariadb,
                   Common::IO::LoggerBase &errLogger /* = Common::IO::ConosoleLogger::err*/) :
    m_errorNumber(error),
    m_node(node),
    m_mariadb(mariadb),
    m_errLogger(errLogger)
{

}

LogEvent::~LogEvent() {

}

void LogEvent::execute() {
    try {
       // NULL for autoincrementing id
       // Node is the node number
       // Error is the error number
       // NULL is for the current timestamp
       const std::string query = "INSERT INTO ctsn.error_log VALUES(NULL, " +
                                 std::to_string(m_node.getID()) + ", " + 
                                 std::to_string(m_errorNumber) + ", NULL);";

       m_mariadb->mysql_real_query(query);
       m_mariadb->mysql_commit();
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

