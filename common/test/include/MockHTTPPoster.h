#ifndef MOCK_HTTP_POSTER_H_
#define MOCK_HTTP_POSTER_H_

#include <gmock/gmock.h>
#include <string>

#include "ctsn_common/HTTPPosterInterface.h"

namespace CTSNCommon {

class MockHTTPPoster : public HTTPPosterInterface {
    public:
        virtual ~MockHTTPPoster(){}

        MOCK_METHOD4(post, void(const std::string &address,
                                const std::string &url,
                                const std::string &data,
                                short port));
};

}

#endif

