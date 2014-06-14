#ifndef PIUART_H
#define PIUART_H

#include <stdexcept>
#include <string>

#include "gateway/UartInterface.h"

namespace Gateway {

class PiUart : public UartInterface
{
    public:
        PiUart();
        ~PiUart();

        /**
         * \throw Runtime error is can not open
         */
        void open (const std::string &file) override;

        /**
         * \throw std::runtime_error if file is not open, or can not be written to
         */
        void send (const std::string &str) override;


        /**
         * \throw std::runtime_error if file is not open, or can not receive data
         */
        std::string recv() override;
        void close() override;

    private:
        static const int NOT_OPEN = -1;
        static const std::string NOT_OPEN_ERROR_MESSAGE;
        static const std::string OPEN_ERROR_MESSAGE;
        static const std::string SEND_ERROR_MESSAGE;
        static const std::string RECV_ERROR_MESSAGE;

        int m_uartFile;
};

}

#endif // PIUART_H
