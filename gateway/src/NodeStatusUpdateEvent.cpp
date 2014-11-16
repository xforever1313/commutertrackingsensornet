#include <memory>
#include <stdexcept>

#include "EventExecutorInterface.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/Node.h"
#include "gateway/NodeStatusUpdateEvent.h"
#include "gateway/NodeContainerInterface.h"
#include "io/LoggerBase.h"

namespace Gateway {

NodeStatusUpdateEvent::NodeStatusUpdateEvent(Node::NodeStatus status,
                                             unsigned int nodeNumber,
                                             NodeContainerInterface *nodes,
                                             Common::EventExecutorInterface *eventExecutor,
                                             MariaDBInterface *mariadb,
                                             Common::IO::LoggerBase &errLogger
                                             /* = Common::IO::ConsoleLogger::err*/) :
    m_status(status),
    m_nodeID(nodeNumber),
    m_nodes(nodes),
    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb),
    m_errLogger(errLogger)
{

}

NodeStatusUpdateEvent::~NodeStatusUpdateEvent() {

}

void NodeStatusUpdateEvent::execute() {
    try {
        // If the status has changed from what it was,
        // log it.
        if (m_nodes->setNodeStatus(m_nodeID, m_status)) {
            ErrorNumber errNumber;

            Node node = m_nodes->getNodeFromID(m_nodeID);

            switch(m_status) {
                case Node::NodeStatus::OKAY:
                    errNumber = NODE_IS_NOW_GOOD;
                    break;
                case Node::NodeStatus::OFFLINE:
                    errNumber = NODE_HAS_GONE_OFFLINE;
                    break;
                case Node::NodeStatus::LOW_BATTERY:
                    errNumber = NODE_HAS_LOW_BATTERY;
                    break;
                case Node::NodeStatus::BATTERY_CRITICAL:
                    errNumber = NODE_HAS_CRITICAL_BATTERY;
                    break;
                case Node::NodeStatus::UNKNOWN:
                    errNumber = NODE_HAS_UNKNOWN_STATUS;
                    break;
                default:
                    errNumber = NODE_HAS_UNCOMMON_STATUS;
                    break;
            }
            std::shared_ptr<ErrorEvent> event(
                new ErrorEvent(errNumber, node, m_mariadb));
            m_eventExecutor->addEvent(event);
        }
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

