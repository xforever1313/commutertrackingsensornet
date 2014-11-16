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
           /* ErrorNumber errNumber;
            switch(m_status) {
                case OKAY:
                    errNumber = NODE_IS_NOW_GOOD;
                    break;
                case OFFLINE:
                    break;
                case LOW_BATTERY:
                    break;
                case BATTERY_CRITICAL:
                    break;
                case UNKNOWN:
                    break;
                default:
                    break;
            };*/
            //std::shared_ptr<ErrorEvent> event(
            //    new ErrorEvent(errNumber, m_node, 
        }
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

