#ifndef UARTTXEVENT_H
#define UARTTXEVENT_H

#include <string>

#include "EventInterface.h"
#include "gateway/UartInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

class UartTxEvent : public Common::EventInterface {
    public:
        UartTxEvent(const std::string &message, UartInterface *uart,
                    Common::IO::LoggerBase &logger = Common::IO::ConsoleLogger::err);
        ~UartTxEvent();

        void execute() override;

    private:
        UartTxEvent() = delete;

        std::string m_message;
        UartInterface *m_uart;
        Common::IO::LoggerBase &m_errorLogger;
};

}

#endif // UARTTXEVENT_H
