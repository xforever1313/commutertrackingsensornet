#ifndef NODE_STATUS_UPDATE_EVENT_H_
#define NODE_STATUS_UPDATE_EVENT_H_

#include "EventExecutorInterface.h"
#include "EventInterface.h"
#include "gateway/MariaDBInterface.h"
#include "ctsn_common/Node.h"
#include "gateway/NodeContainerInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class NodeStatusUpdateEvent
 * \brief This event should be called when a node's status changes.
 *        It will update the database and send an email and text to
 *        the admins.
 */
class NodeStatusUpdateEvent : public Common::EventInterface {
    public:
        /**
         * \brief Constructor
         * \param status The new status for the node
         * \param nodeNumber The node id number that needs to be updated.
         * \param nodes A pointer to a node container object.
         */
        NodeStatusUpdateEvent(CTSNCommon::Node::NodeStatus status,
                              unsigned int nodeNumber,
                              NodeContainerInterface *nodes,
                              Common::EventExecutorInterface *eventExecutor,
                              MariaDBInterface *mariadb,
                              Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~NodeStatusUpdateEvent();

        void execute() override;

    private:
        NodeStatusUpdateEvent() = delete;
        NodeStatusUpdateEvent(const NodeStatusUpdateEvent&) = delete;

        CTSNCommon::Node::NodeStatus m_status;
        unsigned int m_nodeID;
        NodeContainerInterface *m_nodes;
        Common::EventExecutorInterface *m_eventExecutor;
        MariaDBInterface *m_mariadb;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

