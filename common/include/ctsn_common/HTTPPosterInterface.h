#ifndef HTTP_POSTER_INTERFACE_H_
#define HTTP_POSTER_INTERFACE_H_

#include <string>

namespace CTSNCommon {

class HTTPPosterInterface {
    public:
        virtual ~HTTPPosterInterface(){}

        virtual void post(const std::string &address,
                          const std::string &uri,
                          const std::string &data,
                          short portNumber) = 0;
};

}

#endif

