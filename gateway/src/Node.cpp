#include <stdexcept>
#include <string>

#include "gateway/Node.h"

namespace Gateway {

unsigned int Node::numberOfNodes = 1;
const std::string Node::INVALID_NODE_MESSAGE = "Invalid Node";

Node::Node(unsigned int id, uint64_t address) :
    m_id(id),
    m_address(address)
{
}

Node::~Node() {

}

unsigned int Node::getID() const {
    return m_id;
}

uint64_t Node::getAddress() const {
    return m_address;
}

unsigned int Node::convertStringToNodeNumber(const std::string &nodeString) {
    unsigned int nodeNumber = 0;
    try {
        size_t stringSize;
        nodeNumber = std::stoi(nodeString, &stringSize);

        // Ensure theres nothing left over from the nodeString.
        if (stringSize != nodeString.size()) {
            throw std::invalid_argument(INVALID_NODE_MESSAGE);
        }
        // Node can never be zero.
        else if (nodeNumber == 0) {
            throw std::out_of_range(INVALID_NODE_MESSAGE);
        }
        else if (nodeNumber > numberOfNodes) {
            throw std::out_of_range(INVALID_NODE_MESSAGE);
        }
    }
    catch (const std::invalid_argument &e) {
        throw std::invalid_argument(INVALID_NODE_MESSAGE);
    }

    return nodeNumber;
}
 
}

