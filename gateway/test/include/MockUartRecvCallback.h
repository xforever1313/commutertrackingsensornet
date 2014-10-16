#ifndef MOCK_UART_RECV_CALLBACK_H_
#define MOCK_UART_RECV_CALLBACK_H_

#include <cstdint>
#include <gmock/gmock.h>
#include <vector>

#include "gateway/UartRecvCallbackInterface.h"

namespace Gateway {

class MockUartRecvCallback : public UartRecvCallbackInterface {
    public:
        virtual ~MockUartRecvCallback(){}

        MOCK_METHOD1(addData, void(const std::vector<std::uint8_t> &data));
};

}

#endif

