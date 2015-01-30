#include "UnitTest.h"

#include "ctsn_common/PiGPIOController.h"

TEST_GROUP(PiGPIOControllerTest) {

};

TEST(PiGPIOControllerTest, singletonTest) {
    POINTERS_EQUAL(&CTSNCommon::PiGPIOController::getInstance(), &CTSNCommon::PiGPIOController::getInstance());
}
