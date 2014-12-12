#ifndef PICTURE_PARSE_EVENT_H_
#define PICTURE_PARSE_EVENT_H_

#include <string>

#include "ctsn_common/HTTPPosterInterface.h"
#include "EventInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace Gateway {

/**
 * \class PictureParseEvent
 * \brief this is just a passthrough to post to the big computer.
 *        No error checking is done here, thats done on the big
 *        computer, but any http errors will be recorded in the gateway
 *        log however.
 * \warning This is just a temporary workaround.  Consider this deprecated.
 */
class PictureParseEvent : public Common::EventInterface {
    public:
        /**
         * \brief Constructor
         * \param nodeStr the node id in string form
         * \param partStr the part of the picture that was sent
         * \param encodedData base64URL encoded picture data
         */
        PictureParseEvent(const std::string &nodeStr,
                          const std::string &partStr,
                          const std::string &encodedData,
                          CTSNCommon::HTTPPosterInterface *httpPoster,
                          Common::IO::LoggerBase &errLogger =
                            Common::IO::ConsoleLogger::err);

        ~PictureParseEvent();

        void execute() override;

    private:
        std::string m_nodeStr;
        std::string m_partStr;
        std::string m_encodedData;
        CTSNCommon::HTTPPosterInterface *m_httpPoster;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

