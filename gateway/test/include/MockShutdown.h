#ifndef MOCKSHUTDOWN_H_INCLUDED
#define MOCKSHUTDOWN_H_INCLUDED

#include <gmock/gmock.h>

#include "gateway/ShutdownInterface.h"

namespace Gateway {

class MockShutdown : public ShutdownInterface {
    public:
        virtual ~MockShutdown() {}

        MOCK_METHOD0(shutdown, void(void));
};

}

#endif // MOCKSHUTDOWN_H_INCLUDED
