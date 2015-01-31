#include "pi_node/StatusLed.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace PiNode {

StatusLed::StatusLed(unsigned int pinNumber,
                     unsigned int delay,
                     CTSNCommon::GPIOControllerInterface &gpio) :
    m_pinNumber(pinNumber),
    m_delay(delay),
    m_gpio(gpio)
{
}

StatusLed::~StatusLed()
{
    kill();
    join();
}

void StatusLed::kill() {
    m_delayCV.shutdown();
}

bool StatusLed::isShutdown() {
    return m_delayCV.isShutdown();
}

void StatusLed::run() {
    while (!isShutdown()) {
        m_delayCV.timedWait(m_delay);
        m_gpio.set(1, m_pinNumber);
        m_delayCV.timedWait(m_delay);
        m_gpio.set(0, m_pinNumber);
    }
}

}
