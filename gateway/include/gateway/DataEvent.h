#ifndef DATA_EVENT_H_
#define DATA_EVENT_H_

#include "EventInterface.h"
#include "ctsn_common/DataResultTypes.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/Node.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class DataEvent : public Common::EventInterface {
    public:
        DataEvent(const Node &node,
                  CTSNCommon::DataResultType type,
                  MariaDBInterface *mariadb,
                  Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~DataEvent();

        void execute() override;

    private:
        DataEvent() = delete;
        DataEvent(const DataEvent &) = delete;
        DataEvent &operator=(const DataEvent&) = delete;

        Node m_node;
        CTSNCommon::DataResultType m_type;
        MariaDBInterface *m_mariadb;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

