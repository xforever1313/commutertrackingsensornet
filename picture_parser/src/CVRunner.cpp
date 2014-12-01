#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "ctsn_common/DataResultTypes.h"
#include "io/ConsoleLogger.h"
#include "picture_parser/CVRunner.h"

namespace PictureParser {

CVRunner::CVRunner() {

}

CVRunner::~CVRunner() {

}

CTSNCommon::DataResultType CVRunner::parsePicture(const std::vector<uint8_t> &picture) {

    std::string file = "pictureFile";
    std::ofstream outFile(file, std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open file to make picture with");
    }

    for (size_t i = 0; i < picture.size(); ++i) {
        outFile << static_cast<char>(picture[i]);
    }
    outFile.close();

    std::string curlCommand = "curl --data-binary @\"" +
                              file + "\" \"" +
                              "http://localhost:3350/icf/detect.objects?model=pedestrian\"";

    FILE *curlProc = popen(curlCommand.c_str(), "r");

    if (curlProc == nullptr) {
        throw std::runtime_error("Curl failed to open.\n\tCommand: " + curlCommand);
    }
    std::string output;
    char buffer[255];
    while(fgets(buffer, sizeof(buffer), curlProc) != nullptr) {
        output += buffer;
    }

    pclose(curlProc);

    if (output == "[]\n") {
        Common::IO::ConsoleLogger::out.writeLineWithTimeStamp("Unknown object detected");
        return CTSNCommon::DataResultType::UNKNOWN;
    }
    else  {
        Common::IO::ConsoleLogger::out.writeLineWithTimeStamp("Pedestrian Detected");
        return CTSNCommon::DataResultType::WALKER;
    }
}

}

