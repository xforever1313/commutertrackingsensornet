#ifndef CTSN_AUTOTEST_SETTINGS_H_
#define CTSN_AUTOTEST_SETTINGS_H_

#include <stdexcept>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include "ctsn_autotest/CTSNFixtures.h"
#include "SlimList.h"

class CtsnAutoTest {
    public:
        ~CtsnAutoTest();

        static short convertStringToShort(const std::string &s);

        static CtsnAutoTest &getInstance();

        std::string MARIADB_USER;
        std::string MARIADB_PASSWORD;
        short MARIADB_PORT;
        std::string MARIADB_IP;
        std::string MARIADB_DATABASE_NAME;

        std::string GATEWAY_AGENT;
        short GATEWAY_PORT;
        std::string GATEWAY_IP;

        std::string NODE_AGENT;
        short NODE_PORT;
        std::string NODE_IP;

    private:
        CtsnAutoTest();
};

#ifdef __cplusplus
}
#endif


#endif
