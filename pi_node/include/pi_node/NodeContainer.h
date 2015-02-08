#ifndef NODECONTAINER_H
#define NODECONTAINER_H

#include <istream>
#include <map>
#include <mutex>
#include <string>

#include "ctsn_common/Node.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/SettingsParser.h"

namespace PiNode {

class NodeContainer : public CTSNCommon::NodeContainerInterface {
    public:
        NodeContainer();
        ~NodeContainer();

        /**
         * \brief Returns the ID of this node.  Call refreshNodes() first, or this
         *        will probably throw an std::out_of_range since that node doesn't exist yet.
         */
         const CTSNCommon::Node getCurrentNode() override;

        /**
         * \brief gets a node based on the passed in id
         * \param id the id of the node to get.  Pass in a zero to get the broadcast node
         *           (the address for the broadcast node is 0x00000000000000ff, which
         *           will broadcast to all nodes).
         * \throws std::out_of_range if the node number passed in does not exist.
         * \return a COPY of the passed in node.  A reference is not used since the internal map
         *         can be altered by another thread.  This means the returned reference might not last long.
         *         Really, the returned node object should only be used to get the addressing information of a
         *         node before discarding it.
         */
        const CTSNCommon::Node getNodeFromID(unsigned int id) override;

        /**
         * \brief Reads in the node information from the file
         */
        void refreshNodes() override;

    private:
        static const uint64_t BROADCAST_ADDRESS;
        static const std::string INVALID_NODE_MESSAGE;
        static const std::string BAD_NODE_CONFIG;

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

        void refreshNodes(std::istream &in);
        void clearNodes();

        CTSNCommon::Settings &m_settings;
        unsigned int m_currentID; ///  <The ID of the pi node this software is running on.
        std::map<unsigned int, CTSNCommon::Node> m_nodes;
        std::mutex m_nodesMutex;
};

}

#endif
