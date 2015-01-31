#ifndef NODE_CONTAINER_INTERFACE_H_
#define NODE_CONTAINER_INTERFACE_H_

#include <string>

#include "ctsn_common/Node.h"

namespace CTSNCommon {

class NodeContainerInterface {
    public:
        virtual ~NodeContainerInterface(){}

        virtual const CTSNCommon::Node getCurrentNode() = 0;
        virtual const CTSNCommon::Node getNodeFromID(unsigned int id) = 0;
        virtual void refreshNodes() = 0;
        virtual const CTSNCommon::Node convertStringToNode(const std::string &nodeString) = 0;
        virtual bool setNodeStatus(unsigned int id, CTSNCommon::Node::NodeStatus newStatus) = 0;
};

}

#endif

