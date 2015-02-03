#include <map>
#include <memory>

#include "ctsn_common/GPIOControllerInterface.h"
#include "ctsn_common/Node.h"
#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/XBeeHTTPTxEvent.h"
#include "CTSNSharedGlobals.py"
#include "EventExecutorInterface.h"
#include "pi_node/BatteryCheckEvent.h"
#include "pi_node/PinNumbers.h"

namespace PiNode {

BatteryCheckEvent::BatteryCheckEvent(CTSNCommon::NodeContainerInterface *nodes,
                                     Common::EventExecutorInterface *eventExecutor,
                                     CTSNCommon::GPIOControllerInterface &gpio,
                                     CTSNCommon::UartInterface *uart,
                                     Common::IO::LoggerBase &errLogger/* = Common::IO::ConsoleLogger::err*/) :
    m_nodes(nodes),
    m_eventExecutor(eventExecutor),
    m_gpio(gpio),
    m_uart(uart),
    m_errLogger(errLogger)
{
}

BatteryCheckEvent::~BatteryCheckEvent() {
}

void BatteryCheckEvent::execute() {

    try {
        int batteryValue = m_gpio.get(PiNode::PinNumbers::BATTERY_OKAY);
        CTSNCommon::Node node = m_nodes->getCurrentNode();

        std::map<std::string, std::string> data;
        data["node"] = std::to_string(node.getID());

        if (batteryValue == 0) {
            data["status"] = std::to_string(CTSNCommon::Node::BATTERY_CRITICAL);
        }
        else {
            data["status"] = std::to_string(CTSNCommon::Node::OKAY);
        }

        std::shared_ptr<CTSNCommon::XBeeHTTPTxEvent> event (new CTSNCommon::XBeeHTTPTxEvent(node, data, NODE_STATUS_UPDATE_URI, m_uart));
        m_eventExecutor->addEvent(event);
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}
