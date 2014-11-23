#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "UnitTest.h"

#include "b64/encode.h"
#include "b64/decode.h"

TEST_GROUP(b64) {
    TEST_SETUP() {
        m_encoder = new base64::encoder();
        m_decoder = new base64::decoder();
    }

    TEST_TEARDOWN() {
        delete m_encoder;
        delete m_decoder;
    }

    base64::encoder *m_encoder;
    base64::decoder *m_decoder;
};

TEST(b64, test1) {
    std::stringstream input("abcdefghijklmnopqrstuvwxyz");
    std::stringstream encoded;
    std::stringstream decoded;

    m_encoder->encode(input, encoded);
    m_decoder->decode(encoded, decoded);

    CHECK_EQUAL(input.str(), decoded.str());
}

TEST(b64, stressTest) {
    std::stringstream input;
    std::stringstream encoded;
    std::stringstream decoded;

    for (unsigned char c = 1; c < 255; ++c) {
        input << c;
    }
    input << static_cast<unsigned char>(255) << '\0';

    m_encoder->encode(input, encoded);
    m_decoder->decode(encoded, decoded);

    CHECK_EQUAL(input.str(), decoded.str());
}

TEST(b64, carrotTest) {
    CHECK_EQUAL(base64::base64_decode_value('^'), -2);
}

TEST(b64, vectorStressTest) {
    std::vector<uint8_t> input;
    std::string encoded;
    std::vector<uint8_t> decoded;

    for (uint8_t c = 1; c < 255; ++c) {
        input.push_back(c);
    }
    input.push_back(255);
    input.push_back(0);

    encoded = m_encoder->encode(input);
    CHECK(encoded.find('.') == std::string::npos);
    CHECK(encoded.find('=') == std::string::npos);
    CHECK(encoded.find('&') == std::string::npos);
    CHECK(encoded.find('+') == std::string::npos);

    decoded = m_decoder->decode(encoded);

    CHECK_EQUAL(input.size(), decoded.size());

    for (size_t i = 0; i < input.size(); ++i) {
        CHECK_EQUAL(input[i], decoded[i]);
    }
}

TEST(b64, pictureTest1) {
    std::vector<uint8_t> input;
    std::string encoded;
    std::vector<uint8_t> decoded;

    std::string file = "../../../../test/test_card.png";
    std::ifstream inFile(file, std::ios::binary);
    CHECK(inFile.is_open());
    while (!inFile.eof()) {
        input.push_back(inFile.get());
        inFile.peek();
    }

    inFile.close();

    encoded = m_encoder->encode(input);
    CHECK(encoded.find('.') == std::string::npos);
    CHECK(encoded.find('=') == std::string::npos);
    CHECK(encoded.find('&') == std::string::npos);
    CHECK(encoded.find('+') == std::string::npos);

    decoded = m_decoder->decode(encoded);

    CHECK_EQUAL(input.size(), decoded.size());

    for (size_t i = 0; i < input.size(); ++i) {
        CHECK_EQUAL(input[i], decoded[i]);
    }
}

TEST(b64, pictureTest2) {
    std::vector<uint8_t> input;
    std::string encoded;
    std::vector<uint8_t> decoded;

    std::string file = "../../../../test/set_media.png";
    std::ifstream inFile(file, std::ios::binary);
    CHECK(inFile.is_open());
    while (!inFile.eof()) {
        input.push_back(inFile.get());
        inFile.peek();
    }

    inFile.close();

    encoded = m_encoder->encode(input);
    CHECK(encoded.find('.') == std::string::npos);
    CHECK(encoded.find('=') == std::string::npos);
    CHECK(encoded.find('&') == std::string::npos);
    CHECK(encoded.find('+') == std::string::npos);

    decoded = m_decoder->decode(encoded);

    CHECK_EQUAL(input.size(), decoded.size());

    for (size_t i = 0; i < input.size(); ++i) {
        CHECK_EQUAL(input[i], decoded[i]);
    }
}


