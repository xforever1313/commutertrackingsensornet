#ifndef HTTP_POSTER_H_
#define HTTP_POSTER_H_

#include <string>

#include "ctsn_common/HTTPPosterInterface.h"

namespace CTSNCommon {

class HTTPPoster : public HTTPPosterInterface {
    public:
        HTTPPoster();

        ~HTTPPoster();

        /**
         * \brief HTTP Posts the data via curl to the given address.
         * \param address the address to post to. examples are localhost
         *        or 192.168.1.1.  DO NOT include the http://
         * \param uri must start with "/".  It is the part after 
         *        the domain.  So http://localhost:1414/something, pass
         *        in "/something".
         * \param data  Must have the http format.
         *        e.g. node=1&message=hello&type=2
         * \throw std::runtime_error if curl fails to execute
         * \throw std::runtime_error if curl returns a http code
         *        other than 200
         */
        void post (const std::string &address,
                   const std::string &uri,
                   const std::string &data,
                   short portNumber) override;
};

}

#endif

