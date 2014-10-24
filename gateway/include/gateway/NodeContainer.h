#ifndef NODE_CONTAINER_H_
#define NODE_CONTAINER_H_

#include <cstdint>
#include <map>

#include "gateway/MariaDBInterface.h"
#include "gateway/Node.h"
#include "SMutex.h"

namespace Gateway {

/**
 * \brief a static class that contains all the nodes.
 */
class NodeContainer {
    public:
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
        static const Node getNodeFromID(unsigned int id);

        /**
         * \brief refreshes the node map by querying the database.
         * \warning This will LOCK threads entering
         *           getNodeFromID and convertStringToNodeNumber until the operation is complete.
         * \throws std::runtime_error if a database error occurs
         * \throws std::runtime_error if the lengths of the columns from the database somehow do not match.
         * \note throwing an exception will not alter the current node map.  It will also unlock the mutexes.
         */
        static void refreshNodes(const MariaDBInterface *mariadb);

        /**
         * \brief Converts the given string to the node number 
         * \throws std::out_of_range if the node is not a valid node number
         * \throws std::invalid_arugment if the given string is not an int
         */
        static const Node convertStringToNode(const std::string &nodeString);

    private:
        static const uint64_t BROADCAST_ADDRESS;
        static unsigned int numberOfNodes;
        static const std::string INVALID_NODE_MESSAGE;
        static const std::string INVALID_ADDRESS_MESSAGE;
        static std::map<unsigned int, Node> nodes;
        static OS::SMutex nodeMutex;

        /**
         * \brief Clears the node map except for the broadcast node, id 0.
         * \warning this method does not lock the mutex.  
         *          although refreshNodes() calls this, this is mainly here for
         *          the unit tests,
         */
        static void clearNodes();

        NodeContainer() = delete;
        NodeContainer(const NodeContainer&) = delete;
        NodeContainer &operator=(const NodeContainer&) = delete;
};

}

#endif

