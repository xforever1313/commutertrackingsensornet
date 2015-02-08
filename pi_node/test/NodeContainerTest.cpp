#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>

#include "UnitTest.h"

#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/Node.h"
#include "pi_node/NodeContainer.h"

TEST_GROUP(NodeContainerTest) {
    TEST_SETUP() {
        m_settings = &CTSNCommon::Settings::getInstance();
        m_settings->m_shortSettings["NODE_GATEWAY_ID"] = 1;
        m_settings->m_shortSettings["NODE_ID"] = 2;
        m_settings->m_stringSettings["NODE_GATEWAY_ADDRESS"] = "0x01";
        m_settings->m_stringSettings["NODE_ADDRESS"] = "0x02";

        m_uut = new PiNode::NodeContainer();

        //Ensure clearing the nodes leaves 1 node left, the broadcast node.
        //This node has an ID of 0 and an address of 0x000000000000FFFF
        CHECK_EQUAL(m_uut->m_nodes.size(), 1);
        CHECK_EQUAL(m_uut->m_nodes.at(0).getID(), 0);
        CHECK_EQUAL(m_uut->m_nodes.at(0).getAddress(),
                    PiNode::NodeContainer::BROADCAST_ADDRESS);

        // Ensure the current ID does not equal the broadcast id.
        CHECK(m_uut->m_currentID != 0);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    CTSNCommon::Settings *m_settings;
    PiNode::NodeContainer *m_uut;
};

TEST(NodeContainerTest, getNodeFromIDSuccess) {
    CTSNCommon::Node node(1, 0x01);
    m_uut->m_nodes.insert(std::pair<unsigned int, CTSNCommon::Node> (1, node));

    CTSNCommon::Node receivedNode = m_uut->getNodeFromID(node.getID());

    CHECK_EQUAL(receivedNode.getID(), node.getID());
}


TEST(NodeContainerTest, getNodeFromIDFailure) {
    CTSNCommon::Node node(1, 0x01);

    try {
        m_uut->getNodeFromID(node.getID());
        FAIL("Exception should have been thrown");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), PiNode::NodeContainer::INVALID_NODE_MESSAGE + std::to_string(node.getID()));
    }
}

TEST(NodeContainerTest, getCurrentNodeTest) {
    CTSNCommon::Node node(3, 0x01);
    m_uut->m_nodes.insert(std::pair<unsigned int, CTSNCommon::Node> (3, node));

    m_uut->m_currentID = node.getID();

    CTSNCommon::Node receivedNode = m_uut->getCurrentNode();

    CHECK_EQUAL(receivedNode.getID(), node.getID());
}

TEST(NodeContainerTest, refreshTest) {
    m_uut->refreshNodes();

    // Three nodes, broadcast, current, and gateway.
    CHECK_EQUAL(m_uut->m_nodes.size(), 3);

    CHECK_EQUAL(m_uut->m_nodes.at(0).getID(), 0);
    CHECK_EQUAL(m_uut->m_nodes.at(0).getAddress(), PiNode::NodeContainer::BROADCAST_ADDRESS);
    CHECK_EQUAL(m_uut->m_nodes.at(m_settings->getShortSetting("NODE_GATEWAY_ID")).getID(), m_settings->getShortSetting("NODE_GATEWAY_ID"));
    CHECK_EQUAL(m_uut->m_nodes.at(m_settings->getShortSetting("NODE_ID")).getID(), m_settings->getShortSetting("NODE_ID"));
    CHECK_EQUAL(m_uut->m_currentID, m_settings->getShortSetting("NODE_ID"));
}

TEST(NodeContainerTest, refreshTestBad) {
    m_settings->m_stringSettings["NODE_GATEWAY_ADDRESS"] = "0x123derp";

    try {
        m_uut->refreshNodes();
        FAIL("Expected exception");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), PiNode::NodeContainer::BAD_NODE_CONFIG);
    }
}
