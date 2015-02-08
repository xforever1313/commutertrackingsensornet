#include <mysql/mysql.h>
#include <string>
#include <vector>

#include "ctsn_common/SettingsParser.h"
#include "gateway/Gateway.h"
#include "gateway/GatewayDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

bool checkSettings() {
    CTSNCommon::Settings &settings = CTSNCommon::Settings::getInstance();

    // Settings the Gateway needs.
    std::vector<std::string> stringSettings = {"MAILGUN_KEY",
                                               "MAILGUN_URL",
                                               "MAILGUN_FROM",
                                               "MARIADB_USER",
                                               "MARIADB_PASS",
                                               "MARIADB_IP",
                                               "MARIADB_NAME",
                                               "GATEWAY_AGENT",
                                               "GATEWAY_SERIAL",
                                              };

    std::vector<std::string> shortSettings = {"GATEWAY_PORT"};

    return settings.isValid(stringSettings, shortSettings);
}

int main(int argc, char** argv) {
    std::string settingsFile;
    if (argc == 2) {
        settingsFile = argv[1];
    }
    else {
        settingsFile = "settings.xml";
    }

    CTSNCommon::Settings &settings = CTSNCommon::Settings::getInstance();

    try {
        settings.parseFromFile(settingsFile);
        if (!checkSettings()) {
            Common::IO::ConsoleLogger::err.writeLine("There was an error with the xml settings configuration");
            return 1;
        }
    }
    catch (const std::exception &e) {
        Common::IO::ConsoleLogger::err.writeLine(e.what());
        return 2;
    }

    mysql_library_init(argc, argv, nullptr);
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN Gateway Node!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + Gateway::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + Gateway::DateVersion::getVersionNumber());
    Gateway::Gateway &gateway = Gateway::Gateway::getInstance();
    gateway.start();
    mysql_library_end();

    return 0;
}
