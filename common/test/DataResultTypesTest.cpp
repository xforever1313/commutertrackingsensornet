#include <stdexcept>
#include <string>

#include "UnitTest.h"

#include "ctsn_common/DataResultTypes.h"

TEST_GROUP(DataResultTypesTest) {
    TEST_SETUP() {

    }

    TEST_TEARDOWN() {

    }
};

TEST(DataResultTypesTest, goodTest) {
    try {
        CTSNCommon::DataResultType type = 
            CTSNCommon::convertStringToResultType(std::to_string(CTSNCommon::DataResultType::WALKER));

        CHECK_EQUAL(type, CTSNCommon::DataResultType::WALKER);
    }
    catch (...) {
        FAIL("Did not expect exception");
    }
}

TEST(DataResultTypesTest, badStringTest) {
    const std::string badString = "123derp";
    try {
        CTSNCommon::convertStringToResultType(badString);
        FAIL("Expected std::invalid_argument");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::DATA_RESULT_BAD_DATA_TYPE_STRING + badString);
    }

}

TEST(DataResultTypesTest, badStringTest2) {
    const std::string badString = "derp";
    try {
        CTSNCommon::convertStringToResultType(badString);
        FAIL("Expected std::invalid_argument");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::DATA_RESULT_BAD_DATA_TYPE_STRING + badString);
    }

}


TEST(DataResultTypesTest, tooLowTest) {
    const std::string badString = std::to_string(CTSNCommon::DataResultType::BAD_TYPE);
    try {
        CTSNCommon::convertStringToResultType(badString);
        FAIL("Expected std::out_of_range");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::DATA_RESULT_INVALID_DATA_TYPE + badString);
    }
}

TEST(DataResultTypesTest, tooHighTest) {
    const std::string badString = std::to_string(CTSNCommon::DataResultType::TOO_HIGH);
    try {
        CTSNCommon::convertStringToResultType(badString);
        FAIL("Expected std::out_of_range");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), CTSNCommon::DATA_RESULT_INVALID_DATA_TYPE + badString);
    }
}

