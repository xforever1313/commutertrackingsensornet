#include <stdexcept>

#include "pi_node/StatusLed.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace PiNode {

StatusLed::StatusLed(unsigned int pinNumber,
                     unsigned int delay,
                     CTSNCommon::GPIOControllerInterface &gpio,
                     Common::IO::LoggerBase &errLogger/* = Common::IO::ConsoleLogger::err*/) :
    m_pinNumber(pinNumber),
    m_delay(delay),
    m_gpio(gpio),
    m_errLogger(errLogger)
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
    try {
        while (!isShutdown()) {
            m_delayCV.timedWait(m_delay);
            m_gpio.set(1, m_pinNumber);
            m_delayCV.timedWait(m_delay);
            m_gpio.set(0, m_pinNumber);
        }
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
        kill();
    }
}

}
