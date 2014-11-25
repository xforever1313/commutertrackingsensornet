#include <stdexcept>
#include <string>

#include "UnitTest.h"

#include "CTSNSharedGlobals.py"
#include "gateway/PictureParseEvent.h"
#include "io/StringLogger.h"
#include "MockHTTPPoster.h"
#include "Secrets.py"

TEST_GROUP(PictureParseEventTest) {
    TEST_SETUP() {
        m_nodeStr = "2";
        m_partStr = "1";
        m_encodedData = "data";

        m_dataStr = "node=" + m_nodeStr +
                    "&part=" + m_partStr + 
                    "&data=" + m_encodedData;

        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();
        m_errLogger = new Common::IO::StringLogger();
        m_uut = new Gateway::PictureParseEvent(m_nodeStr,
                                               m_partStr,
                                               m_encodedData,
                                               m_httpPoster,
                                               *m_errLogger);

        CHECK_EQUAL(m_uut->m_nodeStr, m_nodeStr);
        CHECK_EQUAL(m_uut->m_partStr, m_partStr);
        CHECK_EQUAL(m_uut->m_encodedData, m_encodedData);
        POINTERS_EQUAL(m_uut->m_httpPoster, m_httpPoster);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_httpPoster;
        delete m_errLogger;
    }

    std::string m_nodeStr;
    std::string m_partStr;
    std::string m_encodedData;
    std::string m_dataStr;

    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;
    Common::IO::StringLogger *m_errLogger;
    Gateway::PictureParseEvent *m_uut;
};

TEST(PictureParseEventTest, successTest) {
    EXPECT_CALL(*m_httpPoster, post(PICTURE_PARSER_IP,
                                    DATA_URI,
                                    m_dataStr,
                                    PICTURE_PARSER_COMMAND_PORT));

    m_uut->execute();

    CHECK(m_errLogger->getString().empty());

}

TEST(PictureParseEventTest, failTest) {
    std::string error = "error";

    EXPECT_CALL(*m_httpPoster, post(PICTURE_PARSER_IP,
                                    DATA_URI,
                                    m_dataStr,
                                    PICTURE_PARSER_COMMAND_PORT))
        .WillOnce(testing::Throw(std::runtime_error(error)));

    m_uut->execute();

    CHECK(m_errLogger->getString().find(error) != 
          std::string::npos);

}
