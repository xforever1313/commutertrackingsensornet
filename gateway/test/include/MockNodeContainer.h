#ifndef MOCK_NODE_CONTAINER_H_
#define MOCK_NODE_CONTAINER_H_

#include <gmock/gmock.h>
#include <string>

#include "gateway/Node.h"
#include "gateway/NodeContainerInterface.h"

namespace Gateway {

class MockNodeContainer : public NodeContainerInterface {
    public:
        virtual ~MockNodeContainer(){}

        MOCK_METHOD1(getNodeFromID, const Node(unsigned int));
        MOCK_METHOD0(refreshNodes, void());
        MOCK_METHOD1(convertStringToNode, const Node(const std::string&));
};

}

#endif

