#include <cstdio> // For popen
#include <stdexcept>
#include <string>

#include "CTSNSharedGlobals.py"
#include "ctsn_common/HTTPPoster.h"

namespace CTSNCommon {

HTTPPoster::HTTPPoster(const std::string &userAgent) :
    m_userAgent(userAgent)
{

}

HTTPPoster::~HTTPPoster() {

}

void HTTPPoster::post(const std::string &address,
                      const std::string &url,
                      const std::string &data,
                      short portNumber) {

    std::string curlCommand = "curl -X POST -A " +
                              m_userAgent +
                              " -sL -w %{http_code}" +
                              " --data \"" +
                              data +
                              "\" http://" + address + ":" +
                              std::to_string(portNumber) +
                              url;


    FILE *f = popen(curlCommand.c_str(), "r");
    if (f == nullptr) {
        throw std::runtime_error("Curl failed to open.\n\tCommand: " + curlCommand);
    }

    std::string output;
    char buffer[255];
    while(fgets(buffer, sizeof(buffer), f) != nullptr) {
        output += buffer;
    }

    pclose(f);

    if (output.find("200") == std::string::npos) {
        throw std::runtime_error("Curl error: \n" + output);
    }

}

}

