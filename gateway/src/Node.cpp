#include <stdexcept>
#include <string>

#include "gateway/Node.h"

namespace Gateway {

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

}

