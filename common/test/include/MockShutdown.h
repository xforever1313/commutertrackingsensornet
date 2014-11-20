#ifndef MOCKSHUTDOWN_H_INCLUDED
#define MOCKSHUTDOWN_H_INCLUDED

#include <gmock/gmock.h>

#include "ctsn_common/ShutdownInterface.h"

namespace CTSNCommon {

class MockShutdown : public ShutdownInterface {
    public:
        virtual ~MockShutdown() {}

        MOCK_METHOD0(shutdown, void(void));
};

}

#endif // MOCKSHUTDOWN_H_INCLUDED
