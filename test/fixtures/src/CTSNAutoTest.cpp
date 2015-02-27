#include <iostream>
#include <stdexcept>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include "ctsn_autotest/CTSNAutoTest.h"
#include "ctsn_autotest/CTSNFixtures.h"

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
    NODE_PORT(0)
{
}

CtsnAutoTest::~CtsnAutoTest(){
}


void *CtsnAutoTest_Create(StatementExecutor *errorHandler, SlimList *args) {
    return &CtsnAutoTest::getInstance();
}

void CtsnAutoTest_Destroy(void *voidSUT) {
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
