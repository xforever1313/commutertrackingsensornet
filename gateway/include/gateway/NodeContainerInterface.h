#ifndef NODE_CONTAINER_INTERFACE_H_
#define NODE_CONTAINER_INTERFACE_H_

#include <string>

#include "gateway/Node.h"

namespace Gateway {

class NodeContainerInterface {
    public:
        virtual ~NodeContainerInterface(){}

        virtual const Node getNodeFromID(unsigned int id) = 0;
        virtual void refreshNodes() = 0;
        virtual const Node convertStringToNode(const std::string &nodeString) = 0;
};

}

#endif

