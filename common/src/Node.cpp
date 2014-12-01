#include <cstdint>
#include <stdexcept>
#include <string>

#include "ctsn_common/Node.h"

namespace CTSNCommon {

const std::string Node::INVALID_NODE_STATUS = "Invalid NodeStatusString - ";

Node::Node(unsigned int id, uint64_t address,
           NodeStatus status) :
    m_id(id),
    m_address(address),
    m_status(status)
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

Node::NodeStatus Node::getStatus() const {
    return m_status;
}

Node::NodeStatus Node::convertStringToNodeStatus(const std::string &s) {
    int status;
    try {
        size_t stringSize;
        status = std::stoi(s, &stringSize);
        if (stringSize != s.size()) {
            throw std::invalid_argument(INVALID_NODE_STATUS + s);
        }
    }
    catch(const std::invalid_argument &e) {
        throw std::invalid_argument(INVALID_NODE_STATUS + s);
    }

    if (status < NodeStatus::OKAY) {
        throw std::out_of_range(INVALID_NODE_STATUS + s);
    }
    else if (status >= NodeStatus::END) {
        throw std::out_of_range(INVALID_NODE_STATUS + s);
    }

    return static_cast<Node::NodeStatus>(status);
}

}

