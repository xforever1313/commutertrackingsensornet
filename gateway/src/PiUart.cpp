#ifdef UNIT_TEST
    #include "MockLinux.h"
    namespace Linux = MockLinux;
#else
    #define Linux //Override the Linux namespace to the global namespace

    void uart_signal_handler(int status) {

    }

#endif

#include <fcntl.h>          //Used for UART
#include <sys/signal.h>     //Used for interrupt
#include <termios.h>        //Used for UART
#include <unistd.h>         //Used for UART

#include <string>

#include "gateway/PiUart.h"

namespace Gateway {

struct sigaction saio;

const std::string PiUart::NOT_OPEN_ERROR_MESSAGE = "You did not open the file!";
const std::string PiUart::OPEN_ERROR_MESSAGE = "Unable to open UART.  Ensure it is not in use by another application.";
const std::string PiUart::SEND_ERROR_MESSAGE = "Unable to send message";
const std::string PiUart::RECV_ERROR_MESSAGE = "Unable to recv message";
const size_t PiUart::BUFFER_SIZE = 255;

PiUart::PiUart() :
    m_uartFile(PiUart::NOT_OPEN)
{
}

PiUart::~PiUart() {
    close();
}


void PiUart::open (const std::string &file) {
    if (m_uartFile == PiUart::NOT_OPEN) {
        m_uartFile = Linux::open(file.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        if (m_uartFile == PiUart::NOT_OPEN) {
            throw std::runtime_error(OPEN_ERROR_MESSAGE);
        }
        else {

            #ifndef UNIT_TEST
                fcntl(m_uartFile, F_SETFL, FNDELAY);
                fcntl(m_uartFile, F_SETOWN, getpid());
                fcntl(m_uartFile, F_SETFL,  O_ASYNC );

                //Make signal:
                saio.sa_handler = uart_signal_handler;
                saio.sa_flags = 0;
                saio.sa_restorer = nullptr;
                sigaction(SIGIO,&saio, nullptr);

                //CONFIGURE THE UART
                //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
                //  Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
                //  CSIZE:- CS5, CS6, CS7, CS8
                //  CLOCAL - Ignore modem status lines
                //  CREAD - Enable receiver
                //  IGNPAR = Ignore characters with parity errors
                //  ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
                //  PARENB - Parity enable
                //  PARODD - Odd parity (else even)
                struct termios options;
                tcgetattr(m_uartFile, &options);
                options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
                options.c_iflag = IGNPAR;
                options.c_oflag = 0;
                options.c_lflag = 0;
                tcflush(m_uartFile, TCIFLUSH);
                tcsetattr(m_uartFile, TCSANOW, &options);

            #endif // UNIT_TEST

        }
    }
}

void PiUart::send (const std::string &str) {
    if (m_uartFile == PiUart::NOT_OPEN) {
        throw std::runtime_error(NOT_OPEN_ERROR_MESSAGE);
    }
    else {
        int byteCount = Linux::write(m_uartFile, str.data(), str.size());

        // write returns 0 if no bytes were written, or less than zero if an error occured.
        if (byteCount < 0) {
            throw std::runtime_error(SEND_ERROR_MESSAGE);
        }
    }
}

std::string PiUart::recv() {
    std::string ret;
    if (m_uartFile == PiUart::NOT_OPEN) {
        throw std::runtime_error(NOT_OPEN_ERROR_MESSAGE);
    }
    else {
        char buffer [BUFFER_SIZE + 1];
        int bytesRead = Linux::read(m_uartFile, buffer, BUFFER_SIZE);
        if (bytesRead < 0) {
            throw std::runtime_error(RECV_ERROR_MESSAGE);
        }
        buffer[bytesRead] = '\0';
        ret = buffer;
    }

    return ret;
}

void PiUart::close() {
    if (m_uartFile != PiUart::NOT_OPEN) {
        m_uartFile = PiUart::NOT_OPEN;
    }
}

}
