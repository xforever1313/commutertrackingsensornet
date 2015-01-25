#include <cstdint>
#include <string>
#include <vector>

#include "UnitTest.h"
#include "ctsn_common/XBeeConstants.h"
#include "ctsn_common/XBeeHTTPTxEvent.h"
#include "io/StringLogger.h"
#include "MockUart.h"

TEST_GROUP(XBeeTxHTTPEventTest){
    TEST_SETUP() {
        m_node = new CTSNCommon::Node(1, 0x0000000000001001);
        m_logger = new Common::IO::StringLogger();
        m_mockUart = new testing::StrictMock<CTSNCommon::MockUart>();

        m_uri = "/myUri";
        m_httpData = {{"node", "1"}, {"data", "2"}};

        m_uut = new CTSNCommon::XBeeHTTPTxEvent(*m_node, m_httpData, m_uri, m_mockUart, *m_logger);

        POINTERS_EQUAL(m_uut->m_uart, m_mockUart);
        CHECK_EQUAL(m_uut->m_node.getID(), m_node->getID());
     }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mockUart;
        delete m_logger;
        delete m_node;
    }

    std::map <std::string, std::string> m_httpData;
    std::string m_uri;

    CTSNCommon::Node *m_node;
    Common::IO::StringLogger *m_logger;
    testing::StrictMock<CTSNCommon::MockUart> *m_mockUart;
    CTSNCommon::XBeeHTTPTxEvent *m_uut;
};

TEST(XBeeTxHTTPEventTest, messageTest) {
    // data comes first since maps sort the key data by alphabetical order.
    std::string expectedString = m_uri + CTSNCommon::XBeeConstants::DATA_SEPARATOR +
                                 "data=2" + CTSNCommon::XBeeConstants::AMP_REPLACE + "node=1";

    CHECK_EQUAL(m_uut->m_message, expectedString);
}

TEST(XBeeTxHTTPEventTest, messageTestNoData) {
    std::string expectedString = m_uri + CTSNCommon::XBeeConstants::DATA_SEPARATOR;

    std::map <std::string, std::string> httpData;
    CTSNCommon::XBeeHTTPTxEvent uut(*m_node, httpData, m_uri, m_mockUart, *m_logger);

    CHECK_EQUAL(uut.m_message, expectedString);
}
