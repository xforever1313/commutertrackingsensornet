#ifndef UARTINTERFACE_INCLUDED
#define UARTINTERFACE_INCLUDED

#include <cstdint>
#include <string>
#include <vector>

namespace CTSNCommon {

class UartInterface {
    public:
        virtual ~UartInterface() {}
        virtual void open (const std::string &file) = 0;
        virtual void send (const std::string &str) = 0;
        virtual void send (const std::vector<std::uint8_t> &data) = 0;
        virtual std::string recvString() = 0;
        virtual std::vector<std::uint8_t> recvBinary() = 0;
        virtual void close() = 0;
};

}

#endif // UARTINTERFACE_INCLUDED
