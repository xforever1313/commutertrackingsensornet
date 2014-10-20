#ifndef MOCK_XBEE_CALLBACK_H_
#define MOCK_XBEE_CALLBACK_H_

#include <cstdint>
#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "gateway/XBeeCallbackInterface.h"

namespace Gateway {

class MockXBeeCallback : public XBeeCallbackInterface {
    public:
        virtual ~MockXBeeCallback(){}

        MOCK_METHOD1(successfulParse, void(const std::string &));
        MOCK_METHOD1(incompleteMessage, void(const std::vector<std::uint8_t>&));
        MOCK_METHOD1(badChecksum, void(const std::vector<std::uint8_t>&));
        MOCK_METHOD1(badState, void(const std::vector<std::uint8_t>&));
};

}

#endif

