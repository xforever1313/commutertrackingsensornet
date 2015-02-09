#include <string>
#include <vector>

#include "pi_node/PiNode.h"
#include "pi_node/PiNodeDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

bool checkSettings() {
    CTSNCommon::Settings &settings = CTSNCommon::Settings::getInstance();

    // Settings the Gateway needs.
    std::vector<std::string> stringSettings = {"NODE_ADDRESS",
                                               "NODE_AGENT",
                                               "NODE_SERIAL",
                                               "NODE_GATEWAY_ADDRESS",
                                              };

    std::vector<std::string> shortSettings = {"NODE_PORT",
                                              "NODE_GATEWAY_ID",
                                              "NODE_ID"
                                             };

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

    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN Pi Node!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + PiNode::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + PiNode::DateVersion::getVersionNumber());

    // Print settings
    Common::IO::ConsoleLogger::out.writeLine(std::string("\nRunning on port: ") + std::to_string(settings.getShortSetting("NODE_PORT")));
    Common::IO::ConsoleLogger::out.writeLine(std::string("Node ID: ") + std::to_string(settings.getShortSetting("NODE_ID")));
    Common::IO::ConsoleLogger::out.writeLine(std::string("Using serial port: ") + settings.getSetting("NODE_SERIAL"));
    Common::IO::ConsoleLogger::out.writeLine(std::string("This node's address: ") + settings.getSetting("NODE_ADDRESS"));
    Common::IO::ConsoleLogger::out.writeLine(std::string("Gateway's address: ") + settings.getSetting("NODE_GATEWAY_ADDRESS"));

    PiNode::PiNode &piNode = PiNode::PiNode::getInstance();
    piNode.start();

    return 0;
}

