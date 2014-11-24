#ifndef PICTURE_PARSE_EVENT_H_
#define PICTURE_PARSE_EVENT_H_

#include <string>

#include "b64/decode.h"
#include "EventInterface.h"
#include "io/LoggerBase.h"
#include "io/ConsoleLogger.h"
#include "ctsn_common/HTTPPosterInterface.h"
#include "picture_parser/CVRunnerInterface.h"
#include "picture_parser/PictureContainer.h"

namespace PictureParser {

class PictureParseEvent : public Common::EventInterface {
    public:
        PictureParseEvent(unsigned int nodeID,
                          unsigned int picturePiece,
                          const std::string &encodedData,
                          PictureContainer *pc,
                          CVRunnerInterface *cvRunner,
                          CTSNCommon::HTTPPosterInterface *httpPoster,
                          Common::IO::LoggerBase &errLogger = 
                              Common::IO::ConsoleLogger::err);

        ~PictureParseEvent();

        void execute() override;

    private:
        static const std::string INVALID_PICTURE_PART;

        unsigned int m_nodeID;
        unsigned int m_picturePiece;
        std::string m_encodedData;
        PictureContainer *m_pc;
        CVRunnerInterface *m_cvRunner;
        base64::decoder *m_decoder;
        CTSNCommon::HTTPPosterInterface *m_httpPoster;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif

