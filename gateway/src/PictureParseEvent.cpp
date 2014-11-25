#include <string>

#include "CTSNSharedGlobals.py"
#include "ctsn_common/HTTPPosterInterface.h"
#include "EventInterface.h"
#include "gateway/PictureParseEvent.h"
#include "io/LoggerBase.h"
#include "Secrets.py"

namespace Gateway {

PictureParseEvent::PictureParseEvent(const std::string &nodeStr,
                                     const std::string &partStr,
                                     const std::string &encodedData,
                                     CTSNCommon::HTTPPosterInterface *httpPoster,
                                     Common::IO::LoggerBase &errLogger
                                        /*=Common::IO::ConsoleLogger::err*/) :
    m_nodeStr(nodeStr),
    m_partStr(partStr),
    m_encodedData(encodedData),
    m_httpPoster(httpPoster),
    m_errLogger(errLogger)
{

}

PictureParseEvent::~PictureParseEvent() {

}

void PictureParseEvent::execute() {
    try {
        std::string dataStr = "node=" + m_nodeStr +
                              "&part=" + m_partStr +
                              "&data=" + m_encodedData;
        m_httpPoster->post(PICTURE_PARSER_IP,
                           DATA_URI,
                           dataStr,
                           PICTURE_PARSER_COMMAND_PORT);
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

