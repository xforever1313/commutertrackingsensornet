#include <cstdint>
#include <vector>

#include "ctsn_common/DataResultTypes.h"
#include "picture_parser/CVRunner.h"

namespace PictureParser {

CVRunner::CVRunner() {

}

CVRunner::~CVRunner() {

}

CTSNCommon::DataResultType CVRunner::parsePicture(const std::vector<uint8_t> &picture) {

    //Jared, implement this method!


    return CTSNCommon::DataResultType::HORSE;
}

}

