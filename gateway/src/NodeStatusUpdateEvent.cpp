#include <stdexcept>

#include "gateway/Node.h"
#include "gateway/NodeStatusUpdateEvent.h"
#include "gateway/NodeContainerInterface.h"
#include "io/LoggerBase.h"

namespace Gateway {

NodeStatusUpdateEvent::NodeStatusUpdateEvent(Node::NodeStatus status,
                                             unsigned int nodeNumber,
                                             NodeContainerInterface *nodes,
                                             Common::IO::LoggerBase &errLogger
                                             /* = Common::IO::ConsoleLogger::err*/) :
    m_status(status),
    m_nodeID(nodeNumber),
    m_nodes(nodes),
    m_errLogger(errLogger)
{

}

NodeStatusUpdateEvent::~NodeStatusUpdateEvent() {

}

void NodeStatusUpdateEvent::execute() {
    try {
        m_nodes->setNodeStatus(m_nodeID, m_status);
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

