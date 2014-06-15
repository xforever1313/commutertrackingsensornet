#ifndef MOCKUART_H_INCLUDED
#define MOCKUART_H_INCLUDED

#include <gmock/gmock.h>
#include <string>

#include "gateway/UartInterface.h"

namespace Gateway {

class MockUart : public UartInterface {
    public:
        virtual ~MockUart() {}

        MOCK_METHOD1(open, void(const std::string&));
        MOCK_METHOD1(send, void(const std::string&));
        MOCK_METHOD0(recv, std::string(void));
        MOCK_METHOD0(close, void(void));
};

}

#endif
