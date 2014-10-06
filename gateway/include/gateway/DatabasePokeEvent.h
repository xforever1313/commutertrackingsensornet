#ifndef DATABASE_POKE_EVENT_H_
#define DATABASE_POKE_EVENT_H_

#include <string>

#include "EventInterface.h"
#include "gateway/MariaDBInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class DatabasePokeEvent : public Common::EventInterface {
    public:
        DatabasePokeEvent(MariaDBInterface *mariadb,
                          Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);
        ~DatabasePokeEvent();

        void execute() override;

    private:
        static const std::string QUERY;

        MariaDBInterface *m_mariadb;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

