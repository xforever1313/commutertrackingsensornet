#ifndef XBEEHTTPTXEVENT_H
#define XBEEHTTPTXEVENT_H

#include <map>
#include <string>

#include "ctsn_common/Node.h"
#include "ctsn_common/XBeeTxEvent.h"
#include "ctsn_common/UartInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace CTSNCommon {

class XBeeHTTPTxEvent : public XBeeTxEvent {
    public:
        /**
         * \param httpData data for an http post request.
         *        usage: httpData[key1] = value1; httpData[key2] = value2; -> key1=value1|key2=value2
         *        (remember, for XBee protocol, | is like & in an http post request, but XBees can not
         *        send & over it)
         * \param uri The uri to send it to.  Remember to add the "/" in front of it.
         */
        XBeeHTTPTxEvent(const Node &node,
                        const std::map<std::string, std::string> &httpData,
                        const std::string &uri,
                        UartInterface *uart,
                        Common::IO::LoggerBase &logger = Common::IO::ConsoleLogger::err);

        ~XBeeHTTPTxEvent();

    private:
        static const std::string convertMapDataToHttpData(const std::map<std::string, std::string> &httpData,
                                                          const std::string &uri);
};

}

#endif
