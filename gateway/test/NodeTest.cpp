#include <CppUTest/TestHarness.h>
#include <stdexcept>
#include <string>

#define private public
#define protected public

#include "gateway/Node.h"

TEST_GROUP(NodeTest) {
    TEST_SETUP() {

    }

    TEST_TEARDOWN() {

    }
};

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

