#ifndef UART_RECV_CALLBACK_INTERFACE_H_
#define UART_RECV_CALLBACK_INTERFACE_H_

#include <cstdint>
#include <vector>

namespace CTSNCommon {

/**
 * \class UartRecvCallbackInterface
 * \brief These are callbacks when uart data is received.
 */
class UartRecvCallbackInterface {
    public:
        virtual ~UartRecvCallbackInterface() {}

        virtual void addData(const std::vector<std::uint8_t> &data) = 0;
};

}

#endif

