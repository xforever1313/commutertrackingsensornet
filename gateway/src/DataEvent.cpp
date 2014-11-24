#include <stdexcept>
#include <string>

#include "gateway/DataEvent.h"
#include "ctsn_common/DataResultTypes.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/Node.h"
#include "io/LoggerBase.h"

namespace Gateway {

DataEvent::DataEvent(const Node &node,
                     CTSNCommon::DataResultType type,
                     MariaDBInterface *mariadb,
                     Common::IO::LoggerBase &errLogger /* = Common::IO::ConsoleLogger::err*/) :
    m_node(node),
    m_type(type),
    m_mariadb(mariadb),
    m_errLogger(errLogger)
{

}

DataEvent::~DataEvent() {

}

void DataEvent::execute() {
    try{
        // Null for id (auto increment)
        // Null for timestamp (current timestamp)
        // Type
        // Node Id.
        m_mariadb->mysql_real_query(std::string("INSERT INTO trail_result VALUES") +
                                    "(NULL, NULL, " + std::to_string(m_type) +
                                    ", " + std::to_string(m_node.getID()) + ")");

        m_mariadb->mysql_commit();
    }
    catch(const std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

