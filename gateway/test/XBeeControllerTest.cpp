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
        CHECK_EQUAL(m_uut->m_dataLength, 0);
        CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
        CHECK_EQUAL(m_uut->m_bytesProcessed, 0);
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
        0x00,           //Garbage data
        0x01,           //Garbage data
        Gateway::XBeeController::START_CHARACTER, // Start Character
        0x00,           // Length1
        0x16,           // Length of 22
        0x10,           // Options
        0x01,           // More options
        0x00,           // Address
        0x00, 
        0x00,
        0x00,
        0x00,
        0x00,
        0xff,
        0xff,
        0Xff,           // Place holder1
        0xfe,           // Place holder2
        0x00,           // Options
        0x00,           // Options
        'H',            // Payload
        'e',
        'l',
        'l',
        'o',
        ' ',
        ':',
        ')',
        0x7c            // Checksum
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
    CHECK_EQUAL(m_uut->m_dataLength, 0);

    // Second character: Garbage data.  Should remain in startup state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 2);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 2);
    CHECK_EQUAL(m_uut->m_dataLength, 0);

    // Third Character: Start character.  Should go to MSG_START state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 3);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 3);
    CHECK_EQUAL(m_uut->m_dataLength, 0);

    // Forth Character: MSB Length.  Should go to LENGTH1 State
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::GOT_LENGTH1);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 4);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 4);
    CHECK_EQUAL(m_uut->m_dataLength, 0); // Length is still zero.


    // Fifth Character: LSB Length.  Should go into LENGTH2 State
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::GOT_LENGTH2);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 5);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 5);
    CHECK_EQUAL(m_uut->m_dataLength, 0x16); // Length is now what is expected.


    std::uint8_t runningCheckSum = 0;
    // 6th -19th characters: ignore this stuff, we dont care until the payload.
    // However, we need to keep track of the checksum at this point.  The Checksum
    // is the sum of all the bits... but only in 8 bit form.
    for (size_t i = 5; i < 19; ++i) {
        runningCheckSum += data[i];

        m_uut->run();
        if (i == 18) {
            // At this point, we want to move on to reading the payload
            CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::PARSE_PAYLOAD);
        }
        else {
            // Still have more characters to ignore!
            CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::IGNORE_OPTIONS);
        }
        CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - i - 1);
        CHECK_EQUAL(m_uut->m_data.size(), data.size() - i - 1);
        CHECK_EQUAL(m_uut->m_checkSumTotal, runningCheckSum);
        CHECK_EQUAL(m_uut->m_bytesProcessed, i - 4);
    }

    // Characters 20 - 27, The payload.  The data we actually care about.
    for (size_t i = 19; i < 27; ++i) {
        runningCheckSum += data[i];

        m_uut->run();
        if (i == 26) {
            //We read the entire payload.  Move on to check the checksum.
            CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::CHECK_CHECKSUM);
            CHECK_EQUAL(m_uut->m_payload, "Hello :)");
        }
        else {
            CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::PARSE_PAYLOAD);
        }
        CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - i - 1);
        CHECK_EQUAL(m_uut->m_data.size(), data.size() - i - 1);
        CHECK_EQUAL(m_uut->m_checkSumTotal, runningCheckSum);
        CHECK_EQUAL(m_uut->m_bytesProcessed, i - 4);
    }

    // LAST CHARACTER! Ensure the checksum is correct.

    //Ensure OUR test checksum is correct
    CHECK_EQUAL(0xff - runningCheckSum, 0x7c);

    // Ensure the uut's checksum is what we expect
    CHECK_EQUAL(0xff - m_uut->m_checkSumTotal, 0x7c);

    /// \todo Add an expect call to a successfull checksum.
    m_uut->run();  // Run the last state.

    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), 0);
    CHECK_EQUAL(m_uut->m_data.size(), 0);

    // Everything should reset
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed, 0);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
}

/**
 * \brief Ensures that handleMessageStartState will make the MSB correct
 */
TEST(XBeeControllerTest, handleMessageStartStateMSBTest) {
    std::vector<std::uint8_t> data = {0xff};
    m_uut->addData(data);
    m_uut->handleMessageStartState();

    CHECK_EQUAL(m_uut->m_dataLength, 0xff00);
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::GOT_LENGTH1);
}

/**
 * \brief Ensures that handleLength1State will make the LSB correct
 */
TEST(XBeeControllerTest, handleGotLength1StateLSBTest) {
    std::vector<std::uint8_t> data = {0xff};
    m_uut->addData(data);
    m_uut->handleGotLength1State();

    CHECK_EQUAL(m_uut->m_dataLength, 0x00ff);
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::GOT_LENGTH2);
}
