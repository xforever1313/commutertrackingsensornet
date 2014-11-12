#include <cstdint>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#include "gateway/MariaDBWrapper.h"
#include "gateway/Node.h"
#include "gateway/NodeContainer.h"
#include "SMutex.h"

namespace Gateway {

const uint64_t NodeContainer::BROADCAST_ADDRESS = 0x000000000000ffff;
const std::string NodeContainer::DATABASE_QUERY = "SELECT * FROM node;";
const std::string NodeContainer::SET_NODE_STATUS_QUERY = "UPDATE node SET update_time=CURRENT_TIMESTAMP,status=";
const std::string NodeContainer::INVALID_NODE_MESSAGE = "Invalid Node - ";
const std::string NodeContainer::INVALID_ADDRESS_MESSAGE = "Invalid Address - ";
const std::string NodeContainer::MISMATCHED_COLUMNS_MESSAGE = "NodeContainer Mismatched columns";
const std::string NodeContainer::INVALID_DATABASE_DATA = "NodeContainer invalid database data";

NodeContainer::NodeContainer(MariaDBInterface *const mariadb) :
    m_mariadb(mariadb),
    m_result(new MariaDBWrapper::MariaDBResult(m_mariadb))
{
    clearNodes();
}

NodeContainer::~NodeContainer() {
    delete m_result;
}

void NodeContainer::refreshNodes() {
    // querty the database
    m_mariadb->mysql_real_query(DATABASE_QUERY);

    //get the results
    m_result->storeResult();
    std::vector<std::string> ids;
    std::vector<std::string> addresses;
    std::vector<std::string> statuses;
    try {
        ids = m_result->getValuesFromColumn("id");
        addresses = m_result->getValuesFromColumn("address");
        statuses = m_result->getValuesFromColumn("status");
        m_result->freeResult();
    }
    catch(const std::invalid_argument &e) {
        m_result->freeResult();
        throw;
    }

    if (ids.size() != addresses.size()) {
        throw std::runtime_error(MISMATCHED_COLUMNS_MESSAGE);
    }
    else if (ids.size() != statuses.size()) {
        throw std::runtime_error(MISMATCHED_COLUMNS_MESSAGE);
    }

    //Time to iterate through the results and create nodes.
    std::map <unsigned int, Node> newNodes;
    for (size_t i = 0; i < ids.size(); ++i) {
        size_t idStringSize = 0;
        size_t addressStringSize = 0;

        unsigned int id;
        uint64_t address;

        try {
            id = std::stol(ids[i], &idStringSize);
            address = std::stoll(addresses[i], &addressStringSize, 16);
        }
        //std::exception so it covers both std::out_of_range and std::argument
        catch (const std::exception &e) {
            throw std::invalid_argument(INVALID_DATABASE_DATA + " id: " + ids[i] + " address: " + addresses[i]);
        }

        // Ensure data is good.
        if (idStringSize != ids[i].size()) {
            throw std::invalid_argument(INVALID_DATABASE_DATA + " id: " + ids[i] + " address: " + addresses[i]);
        }
        else if (addressStringSize != addresses[i].size()) {
            throw std::invalid_argument(INVALID_DATABASE_DATA + " id: " + ids[i] + " address: " + addresses[i]);
        }

        Node::NodeStatus status;
        try {
            status = Node::convertStringToNodeStatus(statuses[i]);
        }
        catch (const std::exception &e) {
            throw std::invalid_argument(INVALID_DATABASE_DATA + " Bad Status: " + statuses[i]);
        }

        Node newNode(id, address, status);
        newNodes.insert(std::pair<unsigned int, Node> (newNode.getID(), newNode));
    }

    //remember to lock the mutex!
    std::lock_guard<OS::SMutex> lock(m_nodeMutex);
    clearNodes();
    m_nodes.insert(newNodes.begin(), newNodes.end());
}

const Node NodeContainer::getNodeFromID(unsigned int id) {
    std::lock_guard<OS::SMutex> lock(m_nodeMutex);
    return m_nodes.at(id);
}

const Node NodeContainer::convertStringToNode(const std::string &nodeString) {
    Node node (0, 0);
    try {
        size_t stringSize;
        unsigned int nodeNumber = std::stol(nodeString, &stringSize);

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
    m_nodes.clear();
    m_nodes.insert(std::pair<unsigned int, Node>(0, Node(0,
                                                         BROADCAST_ADDRESS,
                                                         Node::NodeStatus::OKAY)));
}

bool NodeContainer::setNodeStatus(unsigned int id, 
                                  Node::NodeStatus newStatus) {

    // No need to trouble the database if the passed in status
    // is the same as what is saved.
    if (newStatus == m_nodes.at(id).getStatus()) {
        return false;
    }

    std::string query = SET_NODE_STATUS_QUERY +
                        std::to_string(newStatus) +
                        " WHERE id=" + std::to_string(id);

    // Let the exceptions fly.
    m_mariadb->mysql_real_query(query);
    m_mariadb->mysql_commit();

    // Call refresh.  This will reload in the new status into
    // this cache.
    refreshNodes();
    return true;
}

}

