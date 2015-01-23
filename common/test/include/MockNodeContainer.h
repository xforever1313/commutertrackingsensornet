#ifndef MOCK_NODE_CONTAINER_H_
#define MOCK_NODE_CONTAINER_H_

#include <gmock/gmock.h>
#include <string>

#include "ctsn_common/Node.h"
#include "ctsn_common/NodeContainerInterface.h"

namespace CTSNCommon {

class MockNodeContainer : public NodeContainerInterface {
    public:
        virtual ~MockNodeContainer(){}

        MOCK_METHOD1(getNodeFromID, const CTSNCommon::Node(unsigned int));
        MOCK_METHOD0(refreshNodes, void());
        MOCK_METHOD1(convertStringToNode, const CTSNCommon::Node(const std::string&));
        MOCK_METHOD2(setNodeStatus, bool(unsigned int, CTSNCommon::Node::NodeStatus));
};

}

#endif

