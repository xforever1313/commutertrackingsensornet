#ifndef UARTTXEVENT_H
#define UARTTXEVENT_H

#include <string>

#include "EventInterface.h"
#include "ctsn_common/UartInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class UartTxEvent
 * \brief This event sends data directly out uart.  No preprocessing is done.
 */
class UartTxEvent : public Common::EventInterface {
    public:
        /**
         * \brief Constructor
         * \param message The message to send over uart
         */
        UartTxEvent(const std::string &message, CTSNCommon::UartInterface *uart,
                    Common::IO::LoggerBase &logger = Common::IO::ConsoleLogger::err);
        ~UartTxEvent();

        void execute() override;

    private:
        UartTxEvent() = delete;

        std::string m_message;
        CTSNCommon::UartInterface *m_uart;
        Common::IO::LoggerBase &m_errorLogger;
};

}

#endif // UARTTXEVENT_H
