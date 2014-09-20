#include <CppUTest/TestHarness.h>
#include <stdexcept>
#include <string>

#define private public
#define protected public

#include "gateway/ErrorNumbers.h"

TEST_GROUP(ErrorMessageTest) {
    TEST_SETUP() {

    }

    TEST_TEARDOWN() {

    }
    static const Gateway::ErrorNumber ERROR = Gateway::ErrorNumber::TEST_ERROR;
};

TEST(ErrorMessageTest, convertStringToMessagePass) {
    Gateway::ErrorNumber number = Gateway::ErrorMessage::convertStringToMessage(std::to_string(ERROR));

    CHECK_EQUAL(number, ERROR);
}

TEST(ErrorMessageTest, covertStringToMessageNoNumber) {
    try {
        Gateway::ErrorMessage::convertStringToMessage("abc");
        FAIL("Expected Exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::ErrorMessage::INVALID_MESSAGE);
    }
}

TEST(ErrorMessageTest, covertStringToMessageNumberInFrontOfLetters) {
    try {
        Gateway::ErrorMessage::convertStringToMessage("1abc");
        FAIL("Expected Exception");
    }
    catch (const std::invalid_argument &e) {
        CHECK_EQUAL(e.what(), Gateway::ErrorMessage::INVALID_MESSAGE);
    }
}

TEST(ErrorMessageTest, covertStringToMessageTooSmall) {
    try {
        Gateway::ErrorMessage::convertStringToMessage("0");
        FAIL("Expected Exception");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::ErrorMessage::INVALID_MESSAGE);
    }
}

TEST(ErrorMessageTest, covertStringToMessageTooBig) {
    try {
        Gateway::ErrorMessage::convertStringToMessage("10000");
        FAIL("Expected Exception");
    }
    catch (const std::out_of_range &e) {
        CHECK_EQUAL(e.what(), Gateway::ErrorMessage::INVALID_MESSAGE);
    }
}

