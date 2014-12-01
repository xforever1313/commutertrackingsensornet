#include <CppUTest/TestHarness.h>
#include <stdexcept>
#include <string>

#define private public
#define protected public

#include "ctsn_common/Node.h"

TEST_GROUP(NodeTest) {
    TEST_SETUP() {
        m_id = 1;
        m_address = 0x0013A20040B27E61;
        m_uut = new CTSNCommon::Node(m_id, m_address, 
                                  CTSNCommon::Node::NodeStatus::OKAY);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    unsigned int m_id;
    uint64_t m_address;
    CTSNCommon::Node *m_uut;
};

TEST(NodeTest, getterTest) {
    CHECK_EQUAL(m_uut->getID(), m_id);
    CHECK_EQUAL(m_uut->m_id, m_id);

    CHECK_EQUAL(m_uut->getAddress(), m_address);
    CHECK_EQUAL(m_uut->m_address, m_address);


    CHECK_EQUAL(m_uut->m_status, CTSNCommon::Node::NodeStatus::OKAY);
    CHECK_EQUAL(m_uut->getStatus(), CTSNCommon::Node::NodeStatus::OKAY);
}

TEST(NodeTest, convertStringToNodeStatusSuccess) {
    std::string str = "1";
    try {
        CHECK_EQUAL(CTSNCommon::Node::convertStringToNodeStatus(str), 
                    CTSNCommon::Node::NodeStatus::OKAY);
    }
    catch(const std::exception &e) {
        std::string error = std::string("Did not expect exception: ") + e.what();
        FAIL(error.c_str());
    }

}

TEST(NodeTest, convertStringToNodeStatusTooLow) {
    std::string str = "0";
    try {
        CTSNCommon::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::Node::INVALID_NODE_STATUS + str);
    }
}

TEST(NodeTest, convertStringToNodeStatusTooHigh) {
    std::string str = std::to_string(CTSNCommon::Node::NodeStatus::END);
    try {
        CTSNCommon::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::Node::INVALID_NODE_STATUS + str);
    }
}

TEST(NodeTest, convertStringToNodeStatusBadString) {
    std::string str = "1abc";
    try {
        CTSNCommon::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::Node::INVALID_NODE_STATUS + str);
    }
}

TEST(NodeTest, convertStringToNodeTestBadString2) {
    std::string str = "1abc";
    try {
        CTSNCommon::Node::convertStringToNodeStatus(str);
        FAIL("Expected exception");
    }
    catch(const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::Node::INVALID_NODE_STATUS + str);
    }
}

