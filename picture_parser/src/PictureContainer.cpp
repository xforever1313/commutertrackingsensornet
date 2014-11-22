#include <map>
#include <mutex>

#include "picture_parser/Picture.h"
#include "picture_parser/PictureContainer.h"

namespace PictureParser {

PictureContainer::PictureContainer() {

}

PictureContainer::~PictureContainer() {

}

Picture &PictureContainer::operator[](unsigned int nodeID) {
    std::lock_guard<OS::SMutex> lock(m_picturesMutex);
    if (m_pictures.find(nodeID) == m_pictures.end()) {
        m_pictures[nodeID] = Picture();
    }
    return m_pictures[nodeID];
}

void PictureContainer::removePicture(unsigned int nodeID) {
    std::lock_guard<OS::SMutex> lock(m_picturesMutex);
    m_pictures.erase(nodeID);
}

}

