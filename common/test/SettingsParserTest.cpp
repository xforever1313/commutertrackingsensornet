#include <stdexcept>
#include <string>
#include <vector>

#include "UnitTest.h"

#include "ctsn_common/SettingsParser.h"

const std::string XML_TEST_LOCATION = "../../../../test/xml_test_files/";

TEST_GROUP(SettingsParserTest) {
    TEST_SETUP() {
        m_uut = new CTSNCommon::Settings();   
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    CTSNCommon::Settings *m_uut;
};

TEST(SettingsParserTest, getInstanceTest) {
    POINTERS_EQUAL(&CTSNCommon::Settings::getInstance(),
                   &CTSNCommon::Settings::getInstance());
}

TEST(SettingsParserTest, isValidFailure) {
    // Don't exist yet due to not being parsed
    std::vector<std::string> expectedString = {"NODE_ADDRESS"};
    std::vector<std::string> expectedShort = {"NODE_ID"};

    CHECK(!m_uut->isValid(expectedString, expectedShort));
}

TEST(SettingsParserTest, isValidFailure2) {
    m_uut->m_stringSettings["NODE_ADDRESS"] = "0x001";
    std::vector<std::string> expectedString = {"NODE_ADDRESS"};

    // Don't exist yet due to not being parsed
    std::vector<std::string> expectedShort = {"NODE_ID"};

    CHECK(!m_uut->isValid(expectedString, expectedShort));
}

TEST(SettingsParserTest, isValidThrow) {
    std::vector<std::string> expectedString = {"afsdf"};

    std::vector<std::string> expectedShort = {"NODE_ID"};

    try {
        CHECK(m_uut->isValid(expectedString, expectedShort));
        FAIL("Should have seen an exception");
    }
    catch (const std::out_of_range &e) {

    }
}

TEST(SettingsParserTest, isValidSuccess) {
    m_uut->m_stringSettings["NODE_ADDRESS"] = "0x001";
    std::vector<std::string> expectedString = {"NODE_ADDRESS"};

    m_uut->m_shortSettings["NODE_ID"] = 9000;
    std::vector<std::string> expectedShort = {"NODE_ID"};

    CHECK(m_uut->isValid(expectedString, expectedShort));
}

TEST(SettingsParserTest, badFile) {
    std::string badFile = "derp";
    try {
        m_uut->parseFromFile(badFile);
        FAIL("Should have seen an exception...");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::Settings::FILE_ERROR + badFile);
    }
}

TEST(SettingsParserTest, goodFile) {
    std::string goodFile = XML_TEST_LOCATION + "good.xml";
    try {
        m_uut->parseFromFile(goodFile);
    }
    catch (const std::exception &e) {
        std::string error("Caught exception: ");
        error += e.what();
        FAIL(error.c_str());
    }

    CHECK_EQUAL(m_uut->getSetting("MAILGUN_KEY"), "key");
    CHECK_EQUAL(m_uut->getSetting("MAILGUN_URL"), "url");
    CHECK_EQUAL(m_uut->getSetting("MAILGUN_FROM"), "admin name <postmaster@myURL.com>");
    CHECK_EQUAL(m_uut->getSetting("MARIADB_USER"), "user name");
    CHECK_EQUAL(m_uut->getSetting("MARIADB_PASS"), "pass");
    CHECK_EQUAL(m_uut->getSetting("MARIADB_IP"), "localhost");
    CHECK_EQUAL(m_uut->getSetting("MARIADB_NAME"), "ctsn");
    CHECK_EQUAL(m_uut->getShortSetting("MARIADB_PORT"), 3306);
    CHECK_EQUAL(m_uut->getSetting("GATEWAY_IP"), "ipaddress");
    CHECK_EQUAL(m_uut->getSetting("GATEWAY_AGENT"), "ctsn_user_agent");
    CHECK_EQUAL(m_uut->getSetting("GATEWAY_SERIAL"), "/dev/ttyAMA0");
    CHECK_EQUAL(m_uut->getShortSetting("GATEWAY_PORT"), 1514);
    CHECK_EQUAL(m_uut->getSetting("NODE_AGENT"), "node user agent");
    CHECK_EQUAL(m_uut->getSetting("NODE_SERIAL"), "/dev/ttyAMA0");
    CHECK_EQUAL(m_uut->getShortSetting("NODE_PORT"), 1515);
    CHECK_EQUAL(m_uut->getSetting("NODE_GATEWAY_ADDRESS"), "0x123456789");
    CHECK_EQUAL(m_uut->getShortSetting("NODE_GATEWAY_ID"), 1);
    CHECK_EQUAL(m_uut->getSetting("NODE_ADDRESS"), "0x123456789");
    CHECK_EQUAL(m_uut->getShortSetting("NODE_ID"), 2);
}

TEST(SettingsParserTest, badHeadingTest) {
    std::string goodFile = XML_TEST_LOCATION + "bad_heading.xml";
    try {
        m_uut->parseFromFile(goodFile);
        FAIL("Should have thrown exception...");
    }
    catch (const std::runtime_error &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::Settings::XML_ERROR);
    }
}

TEST(SettingsParserTest, badXML) {
    std::string goodFile = XML_TEST_LOCATION + "bad.xml";
    try {
        m_uut->parseFromFile(goodFile);
        FAIL("Should have thrown exception...");
    }
    catch (const std::exception &e) {
    }
}

