#ifndef CTSN_AUTOTEST_SETTINGS_H_
#define CTSN_AUTOTEST_SETTINGS_H_

#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "SlimList.h"

#ifdef __cplusplus
extern "C" {
#endif

class CtsnAutoTest {
    public:
        ~CtsnAutoTest();

        static short convertStringToShort(const std::string &s);

        static CtsnAutoTest &getInstance();

        // static void runGateway();
        static void runNode();

        // static void killGateway();
        static void killNode();

        void startProc();
        bool waitForProcToEnd();

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

        static std::string OK;
        static std::string FAIL;

    private:
        CtsnAutoTest();

        std::thread *m_gatewayThread;
        std::thread *m_nodeThread;
};

#ifdef __cplusplus
}
#endif


#endif
