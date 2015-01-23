#ifndef NODE_CHECK_EVENT_H_
#define NODE_CHECK_EVENT_H_

#include "ctsn_common/NodeContainerInterface.h"
#include "EventExecutorInterface.h"
#include "EventInterface.h"
#include "gateway/MariaDBInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class NodeCheckEvent
 * \brief This event checks each node in the database to see
 *        when the last time their status was updated.
 *        If its been more than an hour since the last update AND
 *        The node is not offline, this event makes the node's status
 *        "unknown" and creates error events saying which nodes are unknown.
 */
class NodeCheckEvent : public Common::EventInterface {
    public:
        NodeCheckEvent(Common::EventExecutorInterface *eventExecutor,
                       MariaDBInterface *mariadb,
                       CTSNCommon::NodeContainerInterface *nodes,
                       Common::IO::LoggerBase &errLogger =
                           Common::IO::ConsoleLogger::err);

        ~NodeCheckEvent();

        void execute() override;

    private:
        static const std::string OUT_OF_DATE_NODE_QUERY;

        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
        MariaDBResultInterface *m_result;
        CTSNCommon::NodeContainerInterface *m_nodes;
        Common::IO::LoggerBase &m_errLogger;

};

}

#endif

