#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "b64/decode.h"
#include "CTSNSharedGlobals.py"
#include "ctsn_common/HTTPPoster.h"
#include "io/LoggerBase.h"
#include "picture_parser/CVRunnerInterface.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/PictureParseEvent.h"

namespace PictureParser {

const std::string PictureParseEvent::INVALID_PICTURE_PART = 
    "Invalid picture part number.";

PictureParseEvent::PictureParseEvent(unsigned int nodeID,
                                     unsigned int picturePiece,
                                     const std::string &encodedData,
                                     PictureContainer *pc,
                                     CVRunnerInterface *cvRunner,
                                     CTSNCommon::HTTPPosterInterface *httpPoster,
                                     Common::IO::LoggerBase &errLogger
                                        /*=Common::IO::ConsoleLogger::err*/) :
    m_nodeID(nodeID),
    m_picturePiece(picturePiece),
    m_encodedData(encodedData),
    m_pc(pc),
    m_cvRunner(cvRunner),
    m_decoder(new base64::decoder()),
    m_httpPoster(httpPoster),
    m_errLogger(errLogger)
{
}

PictureParseEvent::~PictureParseEvent() {
    delete m_decoder;
}

void PictureParseEvent::execute() {
    try {
        std::vector<uint8_t> decodedData;
        if (m_picturePiece != 0) {
            decodedData = 
                m_decoder->decode(m_encodedData);
        }

        //Clear the encoded data so we save memory
        m_encodedData.clear();

        (*m_pc)[m_nodeID].addData(m_picturePiece, decodedData);

        if ((*m_pc)[m_nodeID].isReadyToGenerate()) {
            std::vector<uint8_t> picture = (*m_pc)[m_nodeID].generatePicture();
            m_pc->removePicture(m_nodeID);
            CTSNCommon::DataResultType type =
                m_cvRunner->parsePicture(picture);

            // GATEWAY_IP is defined at compile time with a -D flag.
            m_httpPoster->post(GATEWAY_IP,
                               DATA_RESULT_URI,
                               "node=" + std::to_string(m_nodeID) +
                               "&type=" + std::to_string(type),
                               GATEWAY_COMMAND_PORT);
        }
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

