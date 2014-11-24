#ifndef MOCK_CV_RUNNER_H_
#define MOCK_CV_RUNNER_H_

#include <cstdint>
#include <gmock/gmock.h>
#include <vector>

#include "ctsn_common/DataResultTypes.h"
#include "picture_parser/CVRunnerInterface.h"

namespace PictureParser {

class MockCVRunner : public CVRunnerInterface {
    public:
        virtual ~MockCVRunner(){}

        MOCK_METHOD1(parsePicture,
                     CTSNCommon::DataResultType(const std::vector<uint8_t> &));
};

}

#endif

