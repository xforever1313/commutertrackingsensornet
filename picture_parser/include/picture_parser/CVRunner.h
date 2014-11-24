#ifndef CV_RUNNER_H_
#define CV_RUNNER_H_

#include <cstdint>
#include <vector>

#include "ctsn_common/DataResultTypes.h"
#include "picture_parser/CVRunnerInterface.h"

namespace PictureParser {

class CVRunner : public CVRunnerInterface {
    public:
        CVRunner();
        ~CVRunner();

        CTSNCommon::DataResultType parsePicture(const std::vector<uint8_t> &picture) override;
};

}

#endif

