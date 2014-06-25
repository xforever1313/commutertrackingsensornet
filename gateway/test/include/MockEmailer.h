#ifndef MOCK_EMAILER_H_
#define MOCK_EMAILER_H_

#include <gmock/gmock.h>

#include "gateway/EmailerInterface.h"

namespace Gateway {

class MockEmailer : public EmailerInterface {
    public:
        virtual ~MockEmailer() {}

        MOCK_METHOD0(send, const std::string());
};

}

#endif
