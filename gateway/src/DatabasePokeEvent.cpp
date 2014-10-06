#include <stdexcept>
#include <string>

#include "gateway/DatabasePokeEvent.h"
#include "gateway/MariaDBInterface.h"
#include "io/LoggerBase.h"

namespace Gateway {

const std::string DatabasePokeEvent::QUERY = "SELECT ID FROM node where ID=1;";

DatabasePokeEvent::DatabasePokeEvent(MariaDBInterface *mariadb,
                                     Common::IO::LoggerBase &errLogger /*= Common::IO::ConsoleLogger::err*/) :
    m_mariadb(mariadb),
    m_errLogger(errLogger)
{
}

DatabasePokeEvent::~DatabasePokeEvent() {

}

void DatabasePokeEvent::execute() {
    try {
        m_mariadb->mysql_real_query(QUERY);
    }
    catch(std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

