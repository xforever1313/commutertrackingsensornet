#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <sstream>

#define private public
#define protected public

#include "gateway/Gateway.h"
#include "io/StringLogger.h"
#include "MockEventExecutor.h"

TEST_GROUP(Gateway) {
    TEST_SETUP() {
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_outputLogger = new Common::IO::StringLogger;

        m_uut = new Gateway::Gateway();
        delete m_uut->m_eventExecutor;
        m_uut->m_eventExecutor = m_eventExecutor;
        m_uut->m_output = m_outputLogger;

    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_outputLogger;
        //Event executor is deleted in gateway.
    }

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    Common::IO::StringLogger *m_outputLogger; //Used to swallow output
    Gateway::Gateway *m_uut;
};


/**
 * \brief Ensures the class exits when exit is selected.
 * \todo When more tests come in, move this to test teardown so the thing exits.
 */
TEST(Gateway, exit) {
    std::stringstream input("0");
    m_uut->m_input = &input;

    m_uut->start();
    //Method should exit for test to pass.
}
