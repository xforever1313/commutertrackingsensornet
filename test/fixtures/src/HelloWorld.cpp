#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include "Fixtures.h"
#include "SlimList.h"

class HelloWorldFixture {
    public:
        HelloWorldFixture() {

        }

        ~HelloWorldFixture() {

        }
};

void *HelloWorld_Create(StatementExecutor *errorHandler, SlimList *args) {
    return new HelloWorldFixture();
}

void HelloWorld_Destroy(void *voidSUT) {
    HelloWorldFixture *sut = reinterpret_cast<HelloWorldFixture*>(voidSUT);
    delete sut;
}

static char *tryThings(void *voidSUT, SlimList *args) {
    static std::string s = "ok";
    return &s[0];
}

SLIM_CREATE_FIXTURE(HelloWorld)
    SLIM_FUNCTION(tryThings)
SLIM_END

#ifdef __cplusplus
}
#endif

