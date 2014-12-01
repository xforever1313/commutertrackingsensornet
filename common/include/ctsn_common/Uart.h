#ifndef PIUART_H
#define PIUART_H

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>

#include "ctsn_common/UartInterface.h"

typedef void(*RecvISR)(int);

namespace CTSNCommon {

class Uart : public UartInterface {
    public:
        /**
         * \param interrupt - a pointer to a function when there
         *                    is data on the uart's receive
         *
         */
        Uart(const RecvISR &recvISR);
        ~Uart();

        /**
         * \throw Runtime error if can not open
         */
        void open (const std::string &file) override;

        /**
         * \brief sends a string of characters to the uart.
         * \throw std::runtime_error if file is not open, or can not be written to
         */
        void send (const std::string &str) override;

        /**
         * \brief Sends raw binary data to the uart.
         * \throw std::runtime_error if file is not open, or can not be written to
         */
         void send (const std::vector<std::uint8_t> &data) override;

        /**
         * \brief Gets up to 255 characters from the UART's receive.
         * \details it is recommended to put this in a while loop until an emptry string is returned.
         * \throw std::runtime_error if file is not open, or can not receive data
         * \return A string of up to 255 characters.  Returns empty string when there is nothing left to read.
         */
         std::string recvString() override;

        /**
         * \brief Gets up to 255 bytes from the UART's receive.
         * \details it is recommended to put this in a while loop until an emptry vector is returned.
         * \throw std::runtime_error if file is not open, or can not receive data
         * \warning unlike recvString(), no null character is appended to the returned vector.
         * \return A vector of up to 255 bytes.  Returns empty vector when there is nothing left to read.
         */
         std::vector<std::uint8_t> recvBinary() override;

        /**
         * \brief closes the Uart
         */
        void close() override;

    private:
        Uart() = delete;

        static const int NOT_OPEN = -1;
        static const std::string NOT_OPEN_ERROR_MESSAGE;
        static const std::string OPEN_ERROR_MESSAGE;
        static const std::string SEND_ERROR_MESSAGE;
        static const std::string RECV_ERROR_MESSAGE;
        static const size_t BUFFER_SIZE;

        int m_uartFile;
        RecvISR m_recvISR;
};

}

#endif // PIUART_H
