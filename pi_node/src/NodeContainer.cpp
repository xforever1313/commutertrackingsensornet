#include <fstream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>

#include "ctsn_common/Node.h"
#include "pi_node/NodeContainer.h"

namespace PiNode {

const uint64_t NodeContainer::BROADCAST_ADDRESS = 0x000000000000ffff;
const std::string NodeContainer::INVALID_NODE_MESSAGE = "Invalid Node - ";
const std::string NodeContainer::NODE_CONFIG_FILE = "node_config.txt";
const std::string NodeContainer::BAD_NODE_CONFIG = "Bad node config file";

NodeContainer::NodeContainer() :
    m_currentID(2) // Make arbitrary 2.  0 and 1 are in use (broadcast and gateway)
{
    clearNodes();
}

NodeContainer::~NodeContainer()
{
    //dtor
}

const CTSNCommon::Node NodeContainer::getCurrentNode() {
    return getNodeFromID(m_currentID);
}

const CTSNCommon::Node NodeContainer::getNodeFromID(unsigned int id) {
    CTSNCommon::Node node (0x00, BROADCAST_ADDRESS);
    try {
        std::lock_guard<std::mutex> lock(m_nodesMutex);
        node = m_nodes.at(id);
    }
    catch(const std::out_of_range &e) {
        throw std::out_of_range(INVALID_NODE_MESSAGE + std::to_string(id));
    }

    return node;
}

void NodeContainer::refreshNodes() {
    std::ifstream inFile (NODE_CONFIG_FILE);
    if (inFile.is_open()) {
        refreshNodes(inFile);
    }
    else {
        throw std::runtime_error("Could not open " + NODE_CONFIG_FILE);
    }
}

bool NodeContainer::setNodeStatus(unsigned int id,
                                  CTSNCommon::Node::NodeStatus newStatus) {
    return false;
}

/**
 * \todo Implement this
 */
const CTSNCommon::Node NodeContainer::convertStringToNode(const std::string &nodeString) {
    return CTSNCommon::Node(0, 0x00);
}

void NodeContainer::clearNodes() {
    m_nodes.clear();
    m_nodes.insert(std::pair<unsigned int, CTSNCommon::Node>(0, CTSNCommon::Node(0,
                                                                                 BROADCAST_ADDRESS,
                                                                                 CTSNCommon::Node::NodeStatus::OKAY)));
}

void NodeContainer::refreshNodes(std::istream &in) {
    std::map <unsigned int, CTSNCommon::Node> nodes;

    in.peek();
    while (!in.eof() && !in.fail()) {
        unsigned int nodeID;
        uint64_t address;

        in >> std::dec >> nodeID;
        in >> std::hex >> address;

        nodes.insert(std::pair<unsigned int, CTSNCommon::Node>(nodeID, CTSNCommon::Node(nodeID, address)));
    }

    if (in.fail() && !in.eof()) {
        throw std::runtime_error(BAD_NODE_CONFIG);
    }

    std::lock_guard<std::mutex> lock(m_nodesMutex);
    clearNodes();
    m_nodes.insert(nodes.begin(), nodes.end());
}

}
