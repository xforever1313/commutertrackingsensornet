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
        m_uut = new Gateway::Node(m_id, m_address);
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
}

TEST(NodeTest, convertStringToNodeSuccess) {
    unsigned int node = Gateway::Node::convertStringToNodeNumber("1");
    CHECK_EQUAL(node, 1);
}

TEST(NodeTest, convertStringToNodeNoNumbersTest) {
    try {
        Gateway::Node::convertStringToNodeNumber("abc");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_MESSAGE);
    }
}

TEST(NodeTest, convertStringToNodeNumberInFrontTest) {
    try {
        Gateway::Node::convertStringToNodeNumber("1abc");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_MESSAGE);
    }
}

TEST(NodeTest, convertStringToNodeNumberTooLowTest) {
    try {
        Gateway::Node::convertStringToNodeNumber("0");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_MESSAGE);
    }
}

TEST(NodeTest, convertStringToNodeNumberTooHighTest) {
    try {
        Gateway::Node::convertStringToNodeNumber("1000000");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::Node::INVALID_NODE_MESSAGE);
    }
}

