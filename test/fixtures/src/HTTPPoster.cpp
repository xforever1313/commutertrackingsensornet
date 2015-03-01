#include <iostream>
#include <stdexcept>
#include <string>

#include "ctsn_autotest/CTSNAutoTest.h"
#include "Fixtures.h"
#include "SlimList.h"

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

void *HttpPoster_Create(StatementExecutor *errorHandler, SlimList *args) {
    return new HttpPoster();
}

void HttpPoster_Destroy(void *voidSUT) {
    HttpPoster *poster = reinterpret_cast<HttpPoster*>(voidSUT);
    delete poster;
}

/**
 * \brief | check |post to | ADDRESS | on port | PORT | to url | URL | with agent | AGENT | and data | derp=true&herp=false | 200 |
 */
static char *postToIpOnPortToUrlWithAgentAndData(void *voidSUT, SlimList *args) {
    HttpPoster *poster = reinterpret_cast<HttpPoster*>(voidSUT);

    std::string ipAddress = SlimList_GetStringAt(args, 0);
    short port = CtsnAutoTest::convertStringToShort(SlimList_GetStringAt(args, 1));
    std::string uri = SlimList_GetStringAt(args, 2);
    std::string agent = SlimList_GetStringAt(args, 3);
    std::string data = SlimList_GetStringAt(args, 4);

    poster->post(ipAddress, uri, data, port, agent);
    return &poster->lastOutput[0];
}

/**
 * \brief | check |post to gateway to url | URL | with data | derp=true&herp=false | 200 |
 */
static char *postToGatewayToUrlWithData(void *voidSUT, SlimList *args) {
    HttpPoster *poster = reinterpret_cast<HttpPoster*>(voidSUT);
    CtsnAutoTest &cat = CtsnAutoTest::getInstance();

    std::string uri = SlimList_GetStringAt(args, 0);
    std::string data = SlimList_GetStringAt(args, 1);

    poster->post(cat.GATEWAY_IP, uri, data, cat.GATEWAY_PORT, cat.GATEWAY_AGENT);
    return &poster->lastOutput[0];
}

/**
 * \brief | check |post to node to url | URL | with data | derp=true&herp=false | 200 |
 */
static char *postToNodeToUrlWithData(void *voidSUT, SlimList *args) {
    HttpPoster *poster = reinterpret_cast<HttpPoster*>(voidSUT);
    CtsnAutoTest &cat = CtsnAutoTest::getInstance();

    std::string uri = SlimList_GetStringAt(args, 0);
    std::string data = SlimList_GetStringAt(args, 1);

    poster->post(cat.NODE_IP, uri, data, cat.NODE_PORT, cat.NODE_AGENT);
    return &poster->lastOutput[0];
}

SLIM_CREATE_FIXTURE(HttpPoster)
    SLIM_FUNCTION(postToIpOnPortToUrlWithAgentAndData)
    SLIM_FUNCTION(postToGatewayToUrlWithData)
    SLIM_FUNCTION(postToNodeToUrlWithData)
SLIM_END

#ifdef __cplusplus
}
#endif
