#include <CppUTest/TestHarness.h>
#include <stdexcept>
#include <string>

#define private public
#define protected public

#include "gateway/Node.h"

TEST_GROUP(NodeTest) {
    TEST_SETUP() {
        m_id = 1;
        m_address = 0x0013A20040B27E61;
        m_uut = new Gateway::Node(m_id, m_address, 
                                  Gateway::Node::NodeStatus::OKAY);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    unsigned int m_id;
    uint64_t m_address;
    Gateway::Node *m_uut;
};

TEST(NodeTest, getterTest) {
    CHECK_EQUAL(m_uut->getID(), m_id);
    CHECK_EQUAL(m_uut->m_id, m_id);

    CHECK_EQUAL(m_uut->getAddress(), m_address);
    CHECK_EQUAL(m_uut->m_address, m_address);


    CHECK_EQUAL(m_uut->m_status, Gateway::Node::NodeStatus::OKAY);
    CHECK_EQUAL(m_uut->getStatus(), Gateway::Node::NodeStatus::OKAY);
}

TEST(NodeTest, convertStringToNodeStatusSuccess) {
    std::string str = "1";
    try {
        CHECK_EQUAL(Gateway::Node::convertStringToNodeStatus(str), 
                    Gateway::Node::NodeStatus::OKAY);
    }
    catch(const std::exception &e) {
        std::string error = std::string("Did not expect exception: ") + e.what();
        FAIL(error.c_str());
    }

}

TEST(NodeTest, convertStringToNodeStatusTooLow) {
    std::string str = "0";
    try {
        Gateway::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_STATUS + str);
    }
}

TEST(NodeTest, convertStringToNodeStatusTooHigh) {
    std::string str = std::to_string(Gateway::Node::NodeStatus::END);
    try {
        Gateway::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_STATUS + str);
    }
}

TEST(NodeTest, convertStringToNodeStatusBadString) {
    std::string str = "1abc";
    try {
        Gateway::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_STATUS + str);
    }
}

TEST(NodeTest, convertStringToNodeTestBadString2) {
    std::string str = "1abc";
    try {
        Gateway::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_STATUS + str);
    }
}

