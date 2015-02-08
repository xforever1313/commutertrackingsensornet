#include <fstream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>

#include "ctsn_common/Node.h"
#include "ctsn_common/SettingsParser.h"
#include "pi_node/NodeContainer.h"

namespace PiNode {

const uint64_t NodeContainer::BROADCAST_ADDRESS = 0x000000000000ffff;
const std::string NodeContainer::INVALID_NODE_MESSAGE = "Invalid Node - ";
const std::string NodeContainer::BAD_NODE_CONFIG = "Bad settings.xml address.";

NodeContainer::NodeContainer() :
    m_settings(CTSNCommon::Settings::getInstance()),
    m_currentID(2) // Make arbitrary 2.  0 and 1 are in use (broadcast and gateway)
{
    clearNodes();
}

NodeContainer::~NodeContainer() {
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
    clearNodes();

    size_t gatewayLength;
    uint64_t gatewayAddress = std::stoi(m_settings.getSetting("NODE_GATEWAY_ADDRESS"), &gatewayLength, 16);

    size_t nodeLength;
    uint64_t nodeAddress = std::stoi(m_settings.getSetting("NODE_ADDRESS"), &nodeLength, 16);

    if ((gatewayLength != m_settings.getSetting("NODE_GATEWAY_ADDRESS").size()) ||
        (nodeLength != m_settings.getSetting("NODE_ADDRESS").size())) {

            throw std::runtime_error(BAD_NODE_CONFIG);
        }

    m_nodes.insert(std::pair<unsigned int, CTSNCommon::Node>(m_settings.getShortSetting("NODE_GATEWAY_ID"),
                                                             CTSNCommon::Node(m_settings.getShortSetting("NODE_GATEWAY_ID"),
                                                                              gatewayAddress,
                                                                              CTSNCommon::Node::NodeStatus::OKAY)));

    m_nodes.insert(std::pair<unsigned int, CTSNCommon::Node>(m_settings.getShortSetting("NODE_ID"),
                                                             CTSNCommon::Node(m_settings.getShortSetting("NODE_ID"),
                                                                              nodeAddress,
                                                                              CTSNCommon::Node::NodeStatus::OKAY)));

    m_currentID = m_settings.getShortSetting("NODE_ID");
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

}
