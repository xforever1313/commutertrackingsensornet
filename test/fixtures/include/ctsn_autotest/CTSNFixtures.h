#include "Fixtures.h"

#ifndef CTSNFIXTURES_H_
#define CTSNFIXTURES_H_

#ifdef __cplusplus
extern "C" {
#endif

SLIM_FIXTURES
    SLIM_FIXTURE(HelloWorld)
    SLIM_FIXTURE(CtsnAutoTest)
    SLIM_FIXTURE(HttpPoster)
SLIM_END

#ifdef __cplusplus
}
#endif

#endif
