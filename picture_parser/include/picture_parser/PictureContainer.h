#ifndef PICTURE_CONTAINER_H_
#define PICTURE_CONTAINER_H_

#include <map>

#include "picture_parser/Picture.h"
#include "SMutex.h"

namespace PictureParser {

class PictureContainer {
    public:
        PictureContainer();

        ~PictureContainer();

        /**
         * \brief returns a reference to a picture object based
         *         on the passed in node id.  If the picture object
         *         does not yet exist, one would be created.
         * \note this is mutex protected.
         */
        Picture &operator[](unsigned int nodeID);

        /**
         * \note this is mutex protected.
         */
        void removePicture(unsigned int nodeID);

    private:
        std::map<unsigned int, Picture> m_pictures;
        OS::SMutex m_picturesMutex;
};

}

#endif

