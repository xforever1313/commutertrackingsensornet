#include <map>
#include <string>

#include "ctsn_common/Node.h"
#include "ctsn_common/XBeeConstants.h"
#include "ctsn_common/XBeeHTTPTxEvent.h"
#include "ctsn_common/XBeeTxEvent.h"
#include "ctsn_common/UartInterface.h"
#include "io/LoggerBase.h"

namespace CTSNCommon {

const std::string XBeeHTTPTxEvent::convertMapDataToHttpData(const std::map<std::string, std::string> &httpData,
                                                            const std::string &uri) {

    std::string message = uri + XBeeConstants::DATA_SEPARATOR;
    for (auto &data : httpData) {
        message += data.first + "=" + data.second + XBeeConstants::AMP_REPLACE;
    }
    if (message[message.size() - 1] == XBeeConstants::AMP_REPLACE) {
        message.erase(message.size() - 1); // ignore the last | as its useless.
    }

    return message;
}

XBeeHTTPTxEvent::XBeeHTTPTxEvent(const Node &node,
                                 const std::map<std::string, std::string> &httpData,
                                 const std::string &uri,
                                 UartInterface *uart,
                                Common::IO::LoggerBase &logger /*= Common::IO::ConsoleLogger::err*/) :
    XBeeTxEvent(node, convertMapDataToHttpData(httpData, uri), uart, logger)
{
}

XBeeHTTPTxEvent::~XBeeHTTPTxEvent()
{
}

}
