#ifndef MOCK_GPIO_CONTROLLER_H_
#define MOCK_GPIO_CONTROLLER_H_

#include <gmock/gmock.h>

#include "ctsn_common/GPIOControllerInterface.h"

namespace CTSNCommon {

class MockGPIOController : public GPIOControllerInterface {
    public:
        virtual ~MockGPIOController() {}

        MOCK_METHOD2(set, void(int newValue, unsigned int pinNumber));
        MOCK_METHOD1(get, int(unsigned int pinNumber));
};

}

#endif
