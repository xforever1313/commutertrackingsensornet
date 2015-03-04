#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include "ctsn_autotest/CTSNAutoTest.h"
#include "ctsn_autotest/CTSNFixtures.h"
#include "ctsn_autotest/HTTPPoster.h"

std::string CtsnAutoTest::OK = "ok";
std::string CtsnAutoTest::FAIL = "fail";


short CtsnAutoTest::convertStringToShort(const std::string &s) {
    size_t length;
    short num = std::stoi(s, &length);
    if (length != s.size()) {
        throw std::runtime_error("Not a short: " + s);
    }

    return num;
}

CtsnAutoTest &CtsnAutoTest::getInstance() {
    static CtsnAutoTest cat;
    return cat;
}

CtsnAutoTest::CtsnAutoTest() :
    MARIADB_PORT(0),
    GATEWAY_PORT(0),
    NODE_PORT(0),
    m_gatewayThread(nullptr),
    m_nodeThread(nullptr)
{
}

CtsnAutoTest::~CtsnAutoTest(){
    m_nodeThread->join();
    delete m_nodeThread;
    delete m_gatewayThread;
}

void CtsnAutoTest::startProc() {
    m_nodeThread = new std::thread(runNode);
}

bool CtsnAutoTest::waitForProcToEnd() {
    m_nodeThread->join();

    return true;
}

void CtsnAutoTest::runNode() {
    std::string commandStr = std::string("ssh ") +
                             getInstance().NODE_IP +
                             " exec \"/home/seth/CTSN/CTSNPiNode /etc/ctsn_settings.xml\" &";
    FILE *f = popen(commandStr.c_str(), "r");
    pclose(f);
}

#ifdef __cplusplus
extern "C" {
#endif

void *CtsnAutoTest_Create(StatementExecutor *errorHandler, SlimList *args) {
    return &CtsnAutoTest::getInstance();
}

void CtsnAutoTest_Destroy(void *voidSUT) {
}

static char *performFixtureSetup(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->startProc();

    bool success = false;
    for (int i = 0; (i < 10) && !success; ++i) {
        // Try to see if the node came up by posting to it
        HttpPoster poster;
        try {
            poster.post(cat->NODE_IP, "/", "herp=derp", cat->NODE_PORT, cat->NODE_AGENT);
        }
        catch(...){}

        success = (poster.lastOutput == "200");

        if (!success) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }

    if (success == false) {
        throw new std::runtime_error("Did not see node come up in time");
    }

    return &CtsnAutoTest::OK[0];
}

static char *performFixtureTeardown(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    bool success = cat->waitForProcToEnd();
    return success ? &CtsnAutoTest::OK[0] : &CtsnAutoTest::FAIL[0];
}

static char *setMariadbUser(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->MARIADB_USER = SlimList_GetStringAt(args, 0);
    return &cat->MARIADB_USER[0];
}

static char *setMariadbPassword(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->MARIADB_PASSWORD = SlimList_GetStringAt(args, 0);
    return &cat->MARIADB_PASSWORD[0];
}

static char *setMariadbPort(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->MARIADB_PORT = CtsnAutoTest::convertStringToShort(SlimList_GetStringAt(args, 0));
    return &std::to_string(cat->MARIADB_PORT)[0];
}

static char *setMariadbIp(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->MARIADB_IP = SlimList_GetStringAt(args, 0);
    return &cat->MARIADB_IP[0];
}

static char *setMariadbDatabaseName(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->MARIADB_DATABASE_NAME = SlimList_GetStringAt(args, 0);
    return &cat->MARIADB_DATABASE_NAME[0];
}

static char *setGatewayAgent(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->GATEWAY_AGENT = SlimList_GetStringAt(args, 0);
    return &cat->GATEWAY_AGENT[0];
}

static char *setGatewayPort(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->GATEWAY_PORT = CtsnAutoTest::convertStringToShort(SlimList_GetStringAt(args, 0));
    return &std::to_string(cat->GATEWAY_PORT)[0];
}

static char *setGatewayIp(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->GATEWAY_IP = SlimList_GetStringAt(args, 0);
    return &cat->GATEWAY_IP[0];
}

static char *setNodeAgent(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->NODE_AGENT = SlimList_GetStringAt(args, 0);
    return &cat->NODE_AGENT[0];
}

static char *setNodePort(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->NODE_PORT = CtsnAutoTest::convertStringToShort(SlimList_GetStringAt(args, 0));
    return &std::to_string(cat->NODE_PORT)[0];
}

static char *setNodeIp(void *voidSUT, SlimList *args) {
    CtsnAutoTest *cat = reinterpret_cast<CtsnAutoTest*>(voidSUT);
    cat->NODE_IP = SlimList_GetStringAt(args, 0);
    return &cat->NODE_IP[0];
}

SLIM_CREATE_FIXTURE(CtsnAutoTest)
    SLIM_FUNCTION(performFixtureSetup)
    SLIM_FUNCTION(performFixtureTeardown)
    SLIM_FUNCTION(setMariadbUser)
    SLIM_FUNCTION(setMariadbPassword)
    SLIM_FUNCTION(setMariadbPort)
    SLIM_FUNCTION(setMariadbIp)
    SLIM_FUNCTION(setMariadbDatabaseName)
    SLIM_FUNCTION(setGatewayAgent)
    SLIM_FUNCTION(setGatewayPort)
    SLIM_FUNCTION(setGatewayIp)
    SLIM_FUNCTION(setNodeAgent)
    SLIM_FUNCTION(setNodePort)
    SLIM_FUNCTION(setNodeIp)
SLIM_END

#ifdef __cplusplus
}
#endif
