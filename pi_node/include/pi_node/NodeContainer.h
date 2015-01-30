#ifndef NODECONTAINER_H
#define NODECONTAINER_H

#include "ctsn_common/Node.h"
#include "ctsn_common/NodeContainerInterface.h"

namespace PiNode {

class NodeContainer : public CTSNCommon::NodeContainerInterface {
    public:
        NodeContainer();
        ~NodeContainer();

        const CTSNCommon::Node getNodeFromID(unsigned int id) override;


    private:
        /**
         * \brief No Op
         */
        void refreshNodes() override;

        /**
         * \brief No Op.  Always returns false.
         * \return false always.
         */
        bool setNodeStatus(unsigned int id,
                           CTSNCommon::Node::NodeStatus newStatus) override;

        /**
         * \todo Implement this
         */
        const CTSNCommon::Node convertStringToNode(const std::string &nodeString) override;

};

}

#endif
