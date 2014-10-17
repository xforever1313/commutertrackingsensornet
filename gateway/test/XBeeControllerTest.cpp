#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include "UnitTest.h"

#include "gateway/XBeeController.h"

TEST_GROUP(XBeeControllerTest) {
    TEST_SETUP() {
        m_uut = new Gateway::XBeeController();

        CHECK(m_uut->isAlive());
        CHECK(m_uut->m_isAlive);
        CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    Gateway::XBeeController *m_uut;
};

TEST(XBeeControllerTest, killTest) {
    m_uut->kill();
    CHECK(!m_uut->isAlive());
    CHECK(!m_uut->m_isAlive);
}

TEST(XBeeControllerTest, verboseStateTest) {
    std::vector<std::uint8_t> data = {
        0x00, //Garbage data
        0x01, //Garbage data
        Gateway::XBeeController::START_CHARACTER, // Start Character
        0x00, // Length
        0xC7 // Checksum
    };

    m_uut->kill(); // Ensure the run loop does not go forever
    m_uut->addData(data); // Add the data to the queue

    // Ensure the semaphore count and the number of stuff in the data
    // queue match up.
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size());
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), m_uut->m_data.size());

    // First character: Garbage data.  Should remain in startup state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 1);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 1);

    // Second character: Garbage data.  Should remain in startup state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 2);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 2);

    // Third Character: Start character.  Should go to MSG_START state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 3);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 3);
}

