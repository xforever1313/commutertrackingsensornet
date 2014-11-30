#ifndef PICTURE_H_
#define PICTURE_H_

#include <cstdint>
#include <string>
#include <map>
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
         * \brief add picture data.
         * \param int the picture part.  Pass in 0 when done.
         * \param data The picture data.
         */
        void addData(unsigned int picturePart, const std::vector<uint8_t> &data);

         /**
          * \return true if both the first and second half have
          *         data in them.
          */
         bool isReadyToGenerate() const;

    private:
        std::map<unsigned int, std::vector<uint8_t> > m_data;
};

}

#endif

