#ifndef CV_RUNNER_INTERFACE_H_
#define CV_RUNNER_INTERFACE_H_

#include <cstdint>
#include <vector>

#include "ctsn_common/DataResultTypes.h"

namespace PictureParser {

class CVRunnerInterface {
    public:
        virtual ~CVRunnerInterface(){}

        virtual CTSNCommon::DataResultType parsePicture(const std::vector<uint8_t> &picture) = 0;
};

}

#endif

