#include <cstdint>
#include <map>
#include <mutex>
#include <string>

#include "gateway/Node.h"
#include "gateway/NodeContainer.h"
#include "SMutex.h"

namespace Gateway {

const uint64_t NodeContainer::BROADCAST_ADDRESS = 0x000000000000ffff;
unsigned int NodeContainer::numberOfNodes = 1;
const std::string NodeContainer::INVALID_NODE_MESSAGE = "Invalid Node - ";
const std::string NodeContainer::INVALID_ADDRESS_MESSAGE = "Invalid Address - ";

OS::SMutex NodeContainer::nodeMutex;
std::map<unsigned int, Node> NodeContainer::nodes;

const Node NodeContainer::getNodeFromID(unsigned int id) {
    std::lock_guard<OS::SMutex> lock(nodeMutex);

    return nodes.at(id);
}

const Node NodeContainer::convertStringToNode(const std::string &nodeString) {
    Node node (0, 0);
    try {
        size_t stringSize;
        unsigned int nodeNumber = std::stoi(nodeString, &stringSize);

        // Ensure theres nothing left over from the nodeString.
        if (stringSize != nodeString.size()) {
            throw std::invalid_argument(INVALID_NODE_MESSAGE + nodeString);
        }

        //std::map.at() will handle nodes too big or do not exist
        //by throwing std::out_of_range.
        node = getNodeFromID(nodeNumber);
    }
    catch (const std::invalid_argument &e) {
        throw std::invalid_argument(INVALID_NODE_MESSAGE + nodeString);
    }
    catch (const std::out_of_range &e) {
        throw std::out_of_range(INVALID_NODE_MESSAGE + nodeString);       
    }
    return node;
}

void NodeContainer::clearNodes() {
    nodes.clear();
    nodes.insert(std::pair<unsigned int, Node>(0, Node(0, BROADCAST_ADDRESS)));
}

}
