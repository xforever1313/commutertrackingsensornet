#include <mysql/mysql.h>
#include <string>

#include "gateway/Gateway.h"
#include "gateway/GatewayDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

int main(int argc, char** argv) {
    mysql_library_init(argc, argv, nullptr);
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN Gateway Node!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + Gateway::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + Gateway::DateVersion::getVersionNumber());
    Gateway::Gateway &gateway = Gateway::Gateway::getInstance();
    gateway.start();
    mysql_library_end();
}
