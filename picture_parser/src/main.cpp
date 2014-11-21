#include <string>

#include "picture_parser/PictureParser.h"
#include "picture_parser/PictureParserDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

int main() {
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN PictureParser!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + PictureParser::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + PictureParser::DateVersion::getVersionNumber());

    PictureParser::PictureParser &pp = PictureParser::PictureParser::getInstance();
    pp.start();

    return 0;
}

