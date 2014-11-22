#include <cstdint>
#include <string>
#include <vector>

#include "UnitTest.h"

#include "picture_parser/Picture.h"

TEST_GROUP(PictureTest) {
    TEST_SETUP() {
        m_firstHalf = {0x00, 0x01, 0x02};
        m_secondHalf = {0x03, 0x04, 0x05};

        m_uut = new PictureParser::Picture();
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    std::vector<uint8_t> m_firstHalf;
    std::vector<uint8_t> m_secondHalf;

    PictureParser::Picture *m_uut;
};

TEST(PictureTest, setFirstHalfTest) {
    m_uut->setFirstHalf(m_firstHalf);
    for (size_t i = 0; i < m_uut->m_firstPictureHalf.size(); ++i) {
        CHECK_EQUAL(m_uut->m_firstPictureHalf[i], m_firstHalf[i]);
    }

    // Only one half is ready, not ready to generate.
    CHECK(!m_uut->isReadyToGenerate());
}

TEST(PictureTest, setSecondHalfTest) {
    m_uut->setSecondHalf(m_secondHalf);
    for (size_t i = 0; i < m_uut->m_secondPictureHalf.size(); ++i) {
        CHECK_EQUAL(m_uut->m_secondPictureHalf[i], m_secondHalf[i]);
    }

    // Only one half is ready, not ready to generate.
    CHECK(!m_uut->isReadyToGenerate());
}

TEST(PictureTest, generatePictureTest) {
    m_uut->setFirstHalf(m_firstHalf);
    m_uut->setSecondHalf(m_secondHalf);

    CHECK(m_uut->isReadyToGenerate());

    std::vector<uint8_t> picture = m_uut->generatePicture();
    for (size_t i = 0; i < (m_firstHalf.size() + m_secondHalf.size()); ++i) {
        if (i < m_firstHalf.size()) {
            CHECK_EQUAL(picture[i], m_firstHalf[i]);
        }
        else {
            CHECK_EQUAL(picture[i], m_secondHalf[i - m_firstHalf.size()]);
        }
    }
}

