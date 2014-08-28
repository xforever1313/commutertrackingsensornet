#include <string>

#include "gateway/Gateway.h"
#include "gateway/GatewayDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

int main() {
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN Gateway Node!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + Gateway::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + Gateway::DateVersion::getVersionNumber());
    Gateway::Gateway &gateway = Gateway::Gateway::getInstance();
    gateway.start();
}
