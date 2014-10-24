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
unsigned int NodeContainer::numberOfNodes = 1;
const std::string NodeContainer::INVALID_NODE_MESSAGE = "Invalid Node - ";
const std::string NodeContainer::INVALID_ADDRESS_MESSAGE = "Invalid Address - ";
const std::string NodeContainer::MISMATCHED_COLUMNS_MESSAGE = "NodeContainer Mismatched columns";
const std::string NodeContainer::INVALID_DATABASE_DATA = "NodeContainer invalid database data";

std::map<unsigned int, Node> NodeContainer::nodes;
OS::SMutex NodeContainer::nodeMutex;
MariaDBResultInterface *NodeContainer::mariadbResult = nullptr;

void NodeContainer::init(MariaDBInterface *const mariadb) {
    static MariaDBWrapper::MariaDBResult result(mariadb);
    NodeContainer::mariadbResult = &result;
}

void NodeContainer::refreshNodes(MariaDBInterface *const mariadb) {
    // querty the database
    mariadb->mysql_real_query(DATABASE_QUERY);

    //get the results
    mariadbResult->storeResult();
    std::vector<std::string> ids;
    std::vector<std::string> addresses;
    try {
        ids = mariadbResult->getValuesFromColumn("id");
        addresses = mariadbResult->getValuesFromColumn("address");
        mariadbResult->freeResult();
    }
    catch(const std::invalid_argument &e) {
        mariadbResult->freeResult();
        throw;
    }

    if (ids.size() != addresses.size()) {
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
        //Exception so it covers both std::out_of_range and std::argument
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
        Node newNode(id, address);
        newNodes.insert(std::pair<unsigned int, Node> (newNode.getID(), newNode));
    }

    //remember to lock the mutex!
    std::lock_guard<OS::SMutex> lock(nodeMutex);
    clearNodes();
    nodes.insert(newNodes.begin(), newNodes.end());
}

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
