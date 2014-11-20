#include <string>

#include "picture_parser/PictureParserDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

int main() {
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN PictureParser!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + PictureParser::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + PictureParser::DateVersion::getVersionNumber());
    return 0;
}

