#include "UnitTest.h"

#include "pi_node/PiNode.h"

TEST_GROUP(PiNode) {

};

TEST(PiNode, getInstanceTest) {
    PiNode::PiNode &node1 = PiNode::PiNode::getInstance();
    PiNode::PiNode &node2 = PiNode::PiNode::getInstance();

    POINTERS_EQUAL(&node1, &node2);
}
