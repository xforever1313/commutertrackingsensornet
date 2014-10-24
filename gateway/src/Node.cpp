#include <stdexcept>
#include <string>

#include "gateway/Node.h"

namespace Gateway {

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

}

