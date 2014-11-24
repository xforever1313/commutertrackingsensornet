#include <cstdio> // For popen
#include <stdexcept>
#include <string>

#include "CTSNSharedGlobals.py"
#include "gateway/HTTPPoster.h"
#include "Secrets.py"

namespace Gateway {

HTTPPoster::HTTPPoster() {

}

HTTPPoster::~HTTPPoster() {

}

void HTTPPoster::post(const std::string &url,
                      const std::string &data,
                      short portNumber) {

    std::string curlCommand = "curl -X POST -A " +
                              USER_AGENT + 
                              " -sL -w %{http_code}" + 
                              " --data \"" +
                              data +
                              "\" http://localhost:" +
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

