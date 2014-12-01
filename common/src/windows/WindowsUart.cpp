#include <string>

#include "gateway/Uart.h"

namespace Gateway {

Uart::Uart(const RecvISR &recvISR) :
    m_uartFile(-1),
    m_recvISR(recvISR)
{
}

Uart::~Uart() {
    close();
}

void Uart::open (const std::string &file) {

}

void Uart::send (const std::string &str) {
    throw std::runtime_error("Not implemented yet");
}

std::string Uart::recv() {
    throw std::runtime_error("Not implemented yet");
}

void Uart::close() {

}

}