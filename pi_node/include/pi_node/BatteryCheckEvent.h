#ifndef BATTERY_CHECK_EVENT_H_
#define BATTERY_CHECK_EVENT_H_

#include "ctsn_common/GPIOControllerInterface.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/UartInterface.h"
#include "EventExecutorInterface.h"
#include "EventInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace PiNode {

class BatteryCheckEvent : public Common::EventInterface {
    public:
        BatteryCheckEvent(CTSNCommon::NodeContainerInterface *nodes,
                          Common::EventExecutorInterface *eventExecutor,
                          CTSNCommon::GPIOControllerInterface &gpio,
                          CTSNCommon::UartInterface *uart,
                          Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~BatteryCheckEvent();

        void execute() override;

    private:
        CTSNCommon::NodeContainerInterface *m_nodes;
        Common::EventExecutorInterface *m_eventExecutor;
        CTSNCommon::GPIOControllerInterface &m_gpio;
        CTSNCommon::UartInterface *m_uart;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif
