#include <string>
#include <vector>

#include "UnitTest.h"

#include "b64/decode.h"
#include "CTSNSharedGlobals.py"
#include "io/StringLogger.h"
#include "MockCVRunner.h"
#include "MockHTTPPoster.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/PictureParseEvent.h"

TEST_GROUP(PictureParseEventTest) {
    TEST_SETUP() {
        m_nodeID = 2;
        m_encodedData = "Helloworldthisisencodeddata";
        m_pc = new PictureParser::PictureContainer();
        m_errLogger = new Common::IO::StringLogger();
        m_cvRunner = new testing::StrictMock<PictureParser::MockCVRunner>();
        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();
        m_uut = new PictureParser::PictureParseEvent(m_nodeID,
                                                     1,
                                                     m_encodedData,
                                                     m_pc,
                                                     m_cvRunner,
                                                     m_httpPoster,
                                                     *m_errLogger);


        POINTERS_EQUAL(m_uut->m_pc, m_pc);
        POINTERS_EQUAL(m_uut->m_cvRunner, m_cvRunner);
        POINTERS_EQUAL(m_uut->m_httpPoster, m_httpPoster);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_cvRunner;
        delete m_pc;
        delete m_errLogger;
        delete m_httpPoster;
    }

    unsigned int m_nodeID;
    std::string m_encodedData;
    PictureParser::PictureContainer *m_pc;
    Common::IO::StringLogger *m_errLogger;
    testing::StrictMock<PictureParser::MockCVRunner> *m_cvRunner;
    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;

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
    CHECK(!(*m_uut->m_pc)[m_nodeID].m_data.at(1).empty());

    CHECK(m_uut->m_encodedData.empty());
    

    // Ready for the second half
    m_uut->m_encodedData = m_encodedData;
    m_uut->m_picturePiece = 2;

    m_uut->execute();

    //Ensure the node was added and exists
    CHECK(m_pc->m_pictures.find(m_nodeID) != m_pc->m_pictures.end());

    // Not ready to generate yet
    CHECK(!(*m_uut->m_pc)[m_nodeID].isReadyToGenerate());

    //Ensure data got filled in.,
    CHECK(!(*m_uut->m_pc)[m_nodeID].m_data.at(1).empty());
    CHECK(!(*m_uut->m_pc)[m_nodeID].m_data.at(2).empty());

    CHECK(m_uut->m_encodedData.empty());


    //Ready to finish
    EXPECT_CALL(*m_cvRunner, parsePicture(testing::_))
        .WillOnce(testing::Return(CTSNCommon::DataResultType::HORSE));

    EXPECT_CALL(*m_httpPoster, post(GATEWAY_IP,
                                    DATA_RESULT_URI,
                                    "node=" + std::to_string(m_nodeID) + 
                                     "&type=" + std::to_string(CTSNCommon::DataResultType::HORSE),
                                    GATEWAY_COMMAND_PORT));

    m_uut->m_encodedData.clear();
    m_uut->m_picturePiece = 0;

    m_uut->execute();

    CHECK(m_pc->m_pictures.find(m_nodeID) == m_pc->m_pictures.end());
}

