#ifndef HTTP_POSTER_H_
#define HTTP_POSTER_H_

#include <iostream>
#include <stdexcept>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

class HttpPoster {
    public:
        HttpPoster() :
            lastOutput("")
        {
        }

        ~HttpPoster()
        {
        }

        void post(const std::string &address,
                  const std::string &url,
                  const std::string &data,
                  short portNumber,
                  const std::string &agent)
        {
            std::string curlCommand = "curl -X POST -A " +
                                      agent +
                                      " -sL -o /dev/null -w %{http_code}" +
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

            std::cout << "Send curl command: " << curlCommand << "\ngot output: " << output << std::endl;

            if (output == "000") {
                throw std::runtime_error("Curl error, could not post.\nCommand:" + curlCommand);
            }

            lastOutput = output;
        }

        /**
         * This is here because we do not want to return
         * in the functions below a string on the stack,
         * where it will go out of scope and have a dangling pointer.
         * instead, have the class have a data member for it
         * so it will never go out of scope.
         * No need to worry about thread safety since FitNesse only has one thread.
         */
        std::string lastOutput;
};

#ifdef __cplusplus
}
#endif

#endif

