#include <cstdint>
#include <string>
#include <vector>

#include "picture_parser/Picture.h"

namespace PictureParser {

Picture::Picture()
{

}

Picture::~Picture() {

}

std::vector<uint8_t> Picture::generatePicture() const {
    std::vector<uint8_t> picture = m_firstPictureHalf;
    for (size_t i = 0; i < m_secondPictureHalf.size(); ++i) {
        picture.push_back(m_secondPictureHalf[i]);
    }
    return picture;
}

void Picture::setFirstHalf(const std::vector<uint8_t> &firstHalf) {
    m_firstPictureHalf = firstHalf;
}

void Picture::setFirstHalfFromBase64(const std::string &firstHalf) {

}

void Picture::setSecondHalf(const std::vector<uint8_t> &secondHalf) {
    m_secondPictureHalf = secondHalf;
}

void Picture::setSecondHalfFromBase64(const std::string &secondHalf) {

}

bool Picture::isReadyToGenerate() const {
    return (!m_firstPictureHalf.empty()) && (!m_secondPictureHalf.empty());
}

}

