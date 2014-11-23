#ifndef PICTURE_H_
#define PICTURE_H_

#include <cstdint>
#include <string>
#include <vector>

namespace PictureParser {

class Picture {
    public:
        Picture();
        ~Picture();

        /**
         * \brief combines the two vectors, into one, and returns it.
         */
        std::vector<uint8_t> generatePicture() const;

        /**
         * \brief sets the first half of the picture.
         */
        void setFirstHalf(const std::vector<uint8_t> &firstHalf);

        /**
         * \brief sets the second half of the picture.
         */
        void setSecondHalf(const std::vector<uint8_t> &secondHalf);

         /**
          * \return true if both the first and second half have
          *         data in them.
          */
         bool isReadyToGenerate() const;

    private:
        std::vector<uint8_t> m_firstPictureHalf;
        std::vector<uint8_t> m_secondPictureHalf;
};

}

#endif

