#ifndef NODE_CONTAINER_H_
#define NODE_CONTAINER_H_

#include <cstdint>
#include <map>

#include "gateway/MariaDBInterface.h"
#include "gateway/NodeContainerInterface.h"
#include "gateway/Node.h"
#include "SMutex.h"

namespace Gateway {

/**
 * \brief a static class that contains all the nodes.
 */
class NodeContainer : public NodeContainerInterface {
    public:
        NodeContainer(MariaDBInterface *const mariadb);

        ~NodeContainer();

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
        const Node getNodeFromID(unsigned int id) override;

        /**
         * \brief refreshes the node map by querying the database.
         * \warning This will LOCK threads entering
         *           getNodeFromID and convertStringToNodeNumber until the operation is complete.
         * \throws std::runtime_error if a database error occurs
         * \throws std::runtime_error if the lengths of the columns from the database somehow do not match.
         * \throws std::invalid_argument if value from database is not correct
                   (e.g. the address is not an int)
         * \note throwing an exception will not alter the current node map.  It will also unlock the mutexes.
         */
        void refreshNodes() override;

        /**
         * \brief Converts the given string to the node number
         * \throws std::out_of_range if the node is not a valid node number
         * \throws std::invalid_arugment if the given string is not an int
         */
        const Node convertStringToNode(const std::string &nodeString) override;

        /**
         * \brief Sets the given node's status
         * \param id the id of the node
         * \param status the status to set the node to
         * \throws std::out_of_range if the passed in node does not exist.
         * \throws std::runtime_error if a database error occurs
         * \note its recommended to call refreshNodes() before calling this function
         * \return true if a database write occurred, false if not.  False is
         *         returned if the node's status stored in the database
         *         is the same as what is passed in.
         */
        bool setNodeStatus(unsigned int id, Node::NodeStatus newStatus) override;

    private:
        static const uint64_t BROADCAST_ADDRESS;
        static const std::string DATABASE_QUERY;
        static const std::string INVALID_NODE_MESSAGE;
        static const std::string INVALID_ADDRESS_MESSAGE;
        static const std::string MISMATCHED_COLUMNS_MESSAGE;
        static const std::string INVALID_DATABASE_DATA;

        /**
         * \brief Clears the node map except for the broadcast node, id 0.
         * \warning this method does not lock the mutex.
         *          although refreshNodes() calls this, this is mainly here for
         *          the unit tests,
         */
        void clearNodes();

        NodeContainer() = delete;
        NodeContainer(const NodeContainer&) = delete;
        NodeContainer &operator=(const NodeContainer&) = delete;

        std::map<unsigned int, Node> m_nodes;
        OS::SMutex m_nodeMutex;
        MariaDBInterface *const m_mariadb;
        MariaDBResultInterface *m_result;
};

}

#endif

