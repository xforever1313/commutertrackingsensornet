#include "UnitTest.h"
#include <cstdint>
#include <map>
#include <string>
#include <stdexcept>

#include "gateway/Node.h"
#include "gateway/NodeContainer.h"

TEST_GROUP(NodeContainerTest) {
    TEST_SETUP() {
        Gateway::NodeContainer::clearNodes();

        //Ensure clearing the nodes leaves 1 node left, the broadcast node.
        //This node has an ID of 0 and an address of 0x000000000000FFFF
        CHECK_EQUAL(Gateway::NodeContainer::nodes.size(), 1);
        CHECK_EQUAL(Gateway::NodeContainer::nodes.at(0).getID(), 0);
        CHECK_EQUAL(Gateway::NodeContainer::nodes.at(0).getAddress(),
                    Gateway::NodeContainer::BROADCAST_ADDRESS);
        Gateway::Node node(1, 0x01);
        Gateway::NodeContainer::nodes.insert(std::pair<unsigned int, Gateway::Node> (1, node));
    }

    TEST_TEARDOWN() {

    }

    
};

TEST(NodeContainerTest, convertStringToNodeSuccess) {
    const Gateway::Node node = Gateway::NodeContainer::convertStringToNode("1");
    CHECK_EQUAL(node.getID(), 1);
}

TEST(NodeContainerTest, convertStringToNodeNoNumbersTest) {
    std::string s = "abc";
    try {
        Gateway::NodeContainer::convertStringToNode(s);
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_NODE_MESSAGE + s);
    }
}

TEST(NodeContainerTest, convertStringToNodeNumberInFrontTest) {
    std::string s = "1abc";
    try {
        Gateway::NodeContainer::convertStringToNode(s);
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_NODE_MESSAGE + s);
    }
}

TEST(NodeContainerTest, convertStringToNodeZeroTest) {
    const Gateway::Node node = Gateway::NodeContainer::convertStringToNode("0");
    CHECK_EQUAL(node.getID(), 0);
    CHECK_EQUAL(node.getAddress(), Gateway::NodeContainer::BROADCAST_ADDRESS);
}

TEST(NodeContainerTest, convertStringToNodeNumberTooHighTest) {
    std::string s = "10000000";
    try {
        Gateway::NodeContainer::convertStringToNode(s);
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::NodeContainer::INVALID_NODE_MESSAGE + s);
    }
}

