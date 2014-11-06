#ifndef HTTP_POSTER_INTERFACE_H_
#define HTTP_POSTER_INTERFACE_H_

#include <string>

namespace Gateway {

class HTTPPosterInterface {
    public:
        virtual ~HTTPPosterInterface(){}

        virtual void post(const std::string &url,
                          const std::string &data) = 0;
};

}

#endif

