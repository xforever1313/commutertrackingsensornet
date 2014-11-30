#include <map>

#include "UnitTest.h"

#include "picture_parser/Picture.h"
#include "picture_parser/PictureContainer.h"

TEST_GROUP(PictureContainerTest) {
    TEST_SETUP() {
        m_uut = new PictureParser::PictureContainer();
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }
    PictureParser::PictureContainer *m_uut;
};

TEST(PictureContainerTest, addRemoveTest) {

    std::cout << "Here" << std::endl;
    CHECK(m_uut->m_pictures.empty());

    PictureParser::Picture &pic1 = (*m_uut)[1]; //Add a new picture.
    CHECK_EQUAL(m_uut->m_pictures.size(), 1);
    CHECK(m_uut->m_pictures.find(1) !=
          m_uut->m_pictures.end());

    PictureParser::Picture &pic2 = (*m_uut)[1]; // Same picture
    CHECK_EQUAL(m_uut->m_pictures.size(), 1);
    CHECK(m_uut->m_pictures.find(1) != 
          m_uut->m_pictures.end());

    //Ensure pointers are the same
    POINTERS_EQUAL(&pic1, &pic2);

    PictureParser::Picture &pic3 = (*m_uut)[2]; // new picture
    CHECK_EQUAL(m_uut->m_pictures.size(), 2);
    CHECK(m_uut->m_pictures.find(2) != 
          m_uut->m_pictures.end());

    //Remove 2nd picture.
    m_uut->removePicture(2);
    CHECK_EQUAL(m_uut->m_pictures.size(), 1);
    CHECK(m_uut->m_pictures.find(2) == 
          m_uut->m_pictures.end());

}

