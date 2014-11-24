#include <cstdint>
#include <string>
#include <vector>

#include "b64/decode.h"
#include "io/LoggerBase.h"
#include "picture_parser/CVRunnerInterface.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/PictureParseEvent.h"

namespace PictureParser {

PictureParseEvent::PictureParseEvent(unsigned int nodeID,
                                     unsigned int picturePiece,
                                     const std::string &encodedData,
                                     PictureContainer *pc,
                                     CVRunnerInterface *cvRunner,
                                     Common::IO::LoggerBase &errLogger
                                        /*=Common::IO::ConsoleLogger::err*/) :
    m_nodeID(nodeID),
    m_picturePiece(picturePiece),
    m_encodedData(encodedData),
    m_pc(pc),
    m_cvRunner(cvRunner),
    m_decoder(new base64::decoder()),
    m_errLogger(errLogger)
{
    delete m_decoder;
}

PictureParseEvent::~PictureParseEvent() {
}

void PictureParseEvent::execute() {
    try {
        if (m_picturePiece == 1) {
            std::vector<uint8_t> decodedData = 
                m_decoder->decode(m_encodedData);

            //Clear the encoded data so we save memory
            m_encodedData.clear();

            (*m_pc)[m_nodeID].setFirstHalf(decodedData);
        }
        else if (m_picturePiece == 2) {
            std::vector<uint8_t> decodedData = 
                m_decoder->decode(m_encodedData);

            //Clear the encoded data so we save memory
            m_encodedData.clear();

            (*m_pc)[m_nodeID].setSecondHalf(decodedData);
        }
        else {

        }

        if ((*m_pc)[m_nodeID].isReadyToGenerate()) {
            std::vector<uint8_t> picture = (*m_pc)[m_nodeID].generatePicture();
            m_pc->removePicture(m_nodeID);
            m_cvRunner->parsePicture(picture);
        }
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}

