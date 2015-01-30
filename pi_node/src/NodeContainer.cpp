#include <string>

#include "ctsn_common/Node.h"
#include "pi_node/NodeContainer.h"

namespace PiNode {

NodeContainer::NodeContainer()
{
    //ctor
}

NodeContainer::~NodeContainer()
{
    //dtor
}

const CTSNCommon::Node NodeContainer::getNodeFromID(unsigned int id) {
    return CTSNCommon::Node(0, 0x00);
}

void NodeContainer::refreshNodes() {

}

bool NodeContainer::setNodeStatus(unsigned int id,
                                  CTSNCommon::Node::NodeStatus newStatus) {
    return false;
}

/**
 * \todo Implement this
 */
const CTSNCommon::Node NodeContainer::convertStringToNode(const std::string &nodeString) {
    return CTSNCommon::Node(0, 0x00);
}

}
