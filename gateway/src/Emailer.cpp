#include <cstdio>
#include <map>
#include <stdexcept>
#include <string>

#include "ctsn_common/SettingsParser.h"
#include "CTSNSharedGlobals.py"
#include "gateway/Emailer.h"

namespace Gateway {

Emailer::Emailer(const std::map <std::string, std::string> &addresses,
                 const std::string &subject, const std::string &message) :
    m_addresses(addresses),
    m_subject(subject),
    m_message(message),
    m_settings(CTSNCommon::Settings::getInstance())
{
}


Emailer::~Emailer() {
}

const std::string Emailer::getCurlCommand() const {
    std::string command = "curl -s --user \"";
    command += m_settings.getSetting("MAILGUN_KEY") + "\" " + m_settings.getSetting("MAILGUN_URL") + " ";
    command += "-F from=\"" + m_settings.getSetting("MAILGUN_FROM") + "\" -F to=\"";

    for (auto &address : m_addresses) {
        command += address.second + " <" + address.first + ">, ";
    }

    command += "\" -F subject=\"" + m_subject + "\" -F text=\"" + m_message + "\"";

    return command;
}

const std::string Emailer::send() {
    std::string ret;

    FILE *f = popen(getCurlCommand().c_str(), "r");

    if (f == nullptr) {
        throw std::runtime_error("EMAIL ERROR: Could not execute curl!");
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), f) != nullptr) {
        ret.append(buffer);
    }

    pclose(f);

    return ret;
}

}
