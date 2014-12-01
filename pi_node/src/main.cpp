#include <string>

#include "pi_node/PiNode.h"
#include "pi_node/PiNodeDateVersion.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

int main() {
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN Pi Node!");
    Common::IO::ConsoleLogger::out.writeLine(std::string("Build: ") + PiNode::DateVersion::getBuildTime());
    Common::IO::ConsoleLogger::out.writeLine(std::string("Version: ") + PiNode::DateVersion::getVersionNumber());

    PiNode::PiNode &piNode = PiNode::PiNode::getInstance();
    piNode.start();

    return 0;
}

