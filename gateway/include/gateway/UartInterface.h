#ifndef UARTINTERFACE_INCLUDED
#define UARTINTERFACE_INCLUDED

#include <string>

namespace Gateway {

class UartInterface {
    public:
        virtual ~UartInterface() {}
        virtual void open (const std::string &file) = 0;
        virtual void send (const std::string &str) = 0;
        virtual std::string recv() = 0;
        virtual void close() = 0;
};

}

#endif // UARTINTERFACE_INCLUDED
