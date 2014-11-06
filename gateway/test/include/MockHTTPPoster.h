#ifndef MOCK_HTTP_POSTER_H_
#define MOCK_HTTP_POSTER_H_

#include <gmock/gmock.h>
#include <string>

#include "gateway/HTTPPosterInterface.h"

namespace Gateway {

class MockHTTPPoster : public HTTPPosterInterface {
    public:
        virtual ~MockHTTPPoster(){}

        MOCK_METHOD2(post, void(const std::string &url, const std::string &data));
};

}

#endif

