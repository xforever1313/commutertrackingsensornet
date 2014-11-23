#include <string>
#include <vector>

#include "UnitTest.h"

#include "b64/decode.h"
#include "io/StringLogger.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/PictureParseEvent.h"

TEST_GROUP(PictureParseEventTest) {
    TEST_SETUP() {
        m_nodeID = 2;
        m_encodedData = "Helloworldthisisencodeddata";
        m_pc = new PictureParser::PictureContainer();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new PictureParser::PictureParseEvent(m_nodeID,
                                                     1,
                                                     m_encodedData,
                                                     m_pc,
                                                     *m_errLogger);

        POINTERS_EQUAL(m_uut->m_pc, m_pc);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_pc;
        delete m_errLogger;
    }

    unsigned int m_nodeID;
    std::string m_encodedData;
    PictureParser::PictureContainer *m_pc;
    Common::IO::StringLogger *m_errLogger;
    base64::decoder m_decoder;

    PictureParser::PictureParseEvent *m_uut;
};

TEST(PictureParseEventTest, successTest) {
    // There is nothing in the picture parser.  Add stuff
    m_uut->execute();

    //Ensure the node was added and exists
    CHECK(m_pc->m_pictures.find(m_nodeID) != m_pc->m_pictures.end());

    // Not ready to generate yet
    CHECK(!(*m_uut->m_pc)[m_nodeID].isReadyToGenerate());

    //Ensure data got filled in.,
    CHECK(!(*m_uut->m_pc)[m_nodeID].m_firstPictureHalf.empty());
    CHECK((*m_uut->m_pc)[m_nodeID].m_secondPictureHalf.empty());


    CHECK(m_uut->m_encodedData.empty());
    

    // Ready for the second half
    m_uut->m_encodedData = m_encodedData;
    m_uut->m_picturePiece = 2;

    m_uut->execute();

    // Ready to generate
    CHECK((*m_uut->m_pc)[m_nodeID].isReadyToGenerate());

    //Ensure data got filled in.,
    CHECK(!(*m_uut->m_pc)[m_nodeID].m_firstPictureHalf.empty());
    CHECK(!(*m_uut->m_pc)[m_nodeID].m_secondPictureHalf.empty());


}

