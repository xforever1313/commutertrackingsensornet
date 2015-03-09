#ifndef STATUS_LED_H_
#define STATUS_LED_H_

#include "ctsn_common/GPIOControllerInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace PiNode {

class StatusLed : public OS::Runnable<StatusLed> {
    public:
        /**
         * \param pinNumber the pin number to turn on and off
         * \param delay the time between flashes in milliseconds
         */
        StatusLed(unsigned int pinNumber,
                  unsigned int delay,
                  CTSNCommon::GPIOControllerInterface &gpio,
                  Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~StatusLed();

        bool isShutdown();

        void kill();

        void run();

    private:
        OS::SConditionVariable m_delayCV;

        unsigned int m_pinNumber;
        unsigned int m_delay;
        CTSNCommon::GPIOControllerInterface &m_gpio;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif // STATUS_LED_H_
