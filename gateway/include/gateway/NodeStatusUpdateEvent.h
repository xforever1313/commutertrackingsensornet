#ifndef NODE_STATUS_UPDATE_EVENT_H_
#define NODE_STATUS_UPDATE_EVENT_H_

#include "EventExecutorInterface.h"
#include "EventInterface.h"
#include "gateway/Node.h"
#include "gateway/NodeContainerInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class NodeStatusUpdateEvent : public Common::EventInterface {
    public:
        NodeStatusUpdateEvent(Node::NodeStatus status, 
                              unsigned int nodeNumber,
                              NodeContainerInterface *nodes,
                              Common::EventExecutorInterface *eventExecutor,
                              Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~NodeStatusUpdateEvent();

        void execute() override;

    private:
        NodeStatusUpdateEvent() = delete;
        NodeStatusUpdateEvent(const NodeStatusUpdateEvent&) = delete;

        Node::NodeStatus m_status;
        unsigned int m_nodeID;
        NodeContainerInterface *m_nodes;
        Common::EventExecutorInterface *m_eventExecutor;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

