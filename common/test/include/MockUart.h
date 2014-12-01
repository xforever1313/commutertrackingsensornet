#ifndef MOCKUART_H_INCLUDED
#define MOCKUART_H_INCLUDED

#include <cstdint>
#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "ctsn_common/UartInterface.h"

namespace CTSNCommon {

class MockUart : public UartInterface {
    public:
        virtual ~MockUart() {}

        MOCK_METHOD1(open, void(const std::string&));
        MOCK_METHOD1(send, void(const std::string&));
        MOCK_METHOD1(send, void(const std::vector<std::uint8_t>&));
        MOCK_METHOD0(recvString, std::string(void));
        MOCK_METHOD0(recvBinary, std::vector<std::uint8_t>(void));
        MOCK_METHOD0(close, void(void));
};

}

#endif
