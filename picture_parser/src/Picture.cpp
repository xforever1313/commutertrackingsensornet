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
    std::vector<uint8_t> picture;
    for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        if (it->first != 0) {
            for (size_t i = 0; i < it->second.size(); ++i) {
                picture.push_back(it->second[i]);
            }
        }
    }
    return picture;
}

void Picture::addData(unsigned int picturePart, const std::vector<uint8_t> &data) {
    m_data.insert(std::pair<unsigned int, std::vector<uint8_t> >(picturePart, data));
}

bool Picture::isReadyToGenerate() const {
    return m_data.find(0) != m_data.end(); 
}

}

