#include <string>

#include "gateway/UartTxEvent.h"
#include "io/LoggerBase.h"

namespace Gateway {

UartTxEvent::UartTxEvent(const std::string &message, UartInterface *uart,
                         Common::IO::LoggerBase &logger /*= Common::IO::ConsoleLogger::err*/) :
    m_message(message),
    m_uart(uart),
    m_errorLogger(logger)
{
}

UartTxEvent::~UartTxEvent() {
}

void UartTxEvent::execute() {
    try {
        m_uart->send(m_message);
    }
    catch (const std::runtime_error &e) {
        m_errorLogger.writeLine(e.what());
    }
}

}
