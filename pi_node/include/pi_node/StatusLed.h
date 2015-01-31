#ifndef STATUS_LED_H_
#define STATUS_LED_H_

#include "ctsn_common/GPIOControllerInterface.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace PiNode {

class StatusLed : public OS::SThread {
    public:
        /**
         * \param pinNumber the pin number to turn on and off
         * \param delay the time between flashes in milliseconds
         */
        StatusLed(unsigned int pinNumber,
                  unsigned int delay,
                  CTSNCommon::GPIOControllerInterface &gpio);

        ~StatusLed();

        bool isShutdown();

        void kill();

    private:
        void run() override;

        OS::SConditionVariable m_delayCV;

        unsigned int m_pinNumber;
        unsigned int m_delay;
        CTSNCommon::GPIOControllerInterface &m_gpio;
};

}

#endif // STATUS_LED_H_
