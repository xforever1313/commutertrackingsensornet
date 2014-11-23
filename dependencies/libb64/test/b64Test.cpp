#include <sstream>

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

