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

