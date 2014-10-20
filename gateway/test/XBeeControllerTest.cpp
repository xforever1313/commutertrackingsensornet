#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include "UnitTest.h"

#include "gateway/XBeeController.h"
#include "MockXBeeCallback.h"

TEST_GROUP(XBeeControllerTest) {
    TEST_SETUP() {
        m_callbacks = new testing::StrictMock<Gateway::MockXBeeCallback>();
        m_uut = new Gateway::XBeeController(m_callbacks);

        CHECK(m_uut->isAlive());
        CHECK(m_uut->m_isAlive);
        CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
        CHECK_EQUAL(m_uut->m_dataLength, 0);
        CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
        CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 0);
        CHECK_EQUAL(m_uut->m_payload, "");
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_callbacks;
    }

    testing::StrictMock<Gateway::MockXBeeCallback> *m_callbacks;
    Gateway::XBeeController *m_uut;
};

TEST(XBeeControllerTest, killTest) {
    m_uut->kill();
    CHECK(!m_uut->isAlive());
    CHECK(!m_uut->m_isAlive);
    CHECK(m_uut->m_dataSemaphore.isShutdown());
}

TEST(XBeeControllerTest, verboseStateTest) {
    std::uint8_t checksum = 0x79;
    std::vector<std::uint8_t> data = {
        0x00,           //Garbage data
        0x01,           //Garbage data
        Gateway::XBeeController::START_CHARACTER, // Start Character
        0x00,           // Length1
        0x14,           // Length of 22
        0x10,           // Frame type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        0Xff,           // Reserved 1
        0xfe,           // Reserved 2
        0x02,           // Recv Options
        'H',            // Payload
        'e',
        'l',
        'l',
        'o',
        ' ',
        ':',
        ')',
        checksum        // Checksum
    };

    m_uut->kill(false); // Ensure the run loop does not go forever
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
    CHECK(m_uut->m_bytesProcessed.empty());

    // Second character: Garbage data.  Should remain in startup state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 2);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 2);
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK(m_uut->m_bytesProcessed.empty());


    // Third Character: Start character.  Should go to MSG_START state.
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 3);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 3);
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], data[2]);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1);


    // Forth Character: MSB Length.  Should go to LENGTH1 State
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::GOT_LENGTH1);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 4);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 4);
    CHECK_EQUAL(m_uut->m_dataLength, 0); // Length is still zero.
    CHECK_EQUAL(m_uut->m_bytesProcessed[1], data[3]);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 2);


    // Fifth Character: LSB Length.  Should go into LENGTH2 State
    m_uut->run();
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::GOT_LENGTH2);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), data.size() - 5);
    CHECK_EQUAL(m_uut->m_data.size(), data.size() - 5);
    CHECK_EQUAL(m_uut->m_dataLength, 0x14); // Length is now what is expected.
    CHECK_EQUAL(m_uut->m_bytesProcessed[2], data[4]);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 3);


    std::uint8_t runningCheckSum = 0;
    // 5th -17th characters: ignore this stuff, we dont care until the payload.
    // However, we need to keep track of the checksum at this point.  The Checksum
    // is the sum of all the bits... but only in 8 bit form.
    for (size_t i = 5; i < 17; ++i) {
        runningCheckSum += data[i];

        m_uut->run();
        if (i == 16) {
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
        CHECK_EQUAL(m_uut->m_bytesProcessed[i - 2], data[i]);
        CHECK_EQUAL(m_uut->m_bytesProcessed.size(), i - 1); 
    }

    // Characters 20 - 27, The payload.  The data we actually care about.
    for (size_t i = 17; i < 25; ++i) {
        runningCheckSum += data[i];

        m_uut->run();
        if (i == 24) {
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
        CHECK_EQUAL(m_uut->m_bytesProcessed[i - 2], data[i]);
        CHECK_EQUAL(m_uut->m_bytesProcessed.size(), i - 1);
    }

    // LAST CHARACTER! Ensure the checksum is correct.

    //Ensure OUR test checksum is correct
    CHECK_EQUAL(0xff - runningCheckSum, checksum);

    // Ensure the uut's checksum is what we expect
    CHECK_EQUAL(0xff - m_uut->m_checkSumTotal, checksum);

    // Ensure the callback will get called when the checksum is good.
    EXPECT_CALL(*m_callbacks, successfulParse("Hello :)"));

    m_uut->run();  // Run the last state.

    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
    CHECK_EQUAL(m_uut->m_dataSemaphore.getSemaphoreCount(), 0);
    CHECK_EQUAL(m_uut->m_data.size(), 0);

    // Everything should reset
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 0);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
    CHECK_EQUAL(m_uut->m_payload, "");
}

/**
 * \brief tests to see that a bad checksum will put the thing in the right state.
 */
TEST(XBeeControllerTest, badChecksumTest) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER, // Start Character
        0x00,           // Length1
        0x14,           // Length of 20
        0x10,           // Frame type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        0Xff,           // Reserved 1
        0xfe,           // Reserved 2
        0x02,           // Recv Options
        'H',            // Payload
        'e',
        'l',
        'l',
        'o',
        ' ',
        ':',
        ')',
        0x70            // Checksum
    };

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, badChecksum(data))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should reset
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 0);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
    CHECK_EQUAL(m_uut->m_payload, "");
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
}

/////
/// Incomplete Message Tests
/////
TEST(XBeeControllerTest, incompleteMessageStartupState) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        Gateway::XBeeController::START_CHARACTER // Premature start char
    };

    // Get the expected data to be passed through
    std::vector<std::uint8_t> expectedPrintedData = data;
    expectedPrintedData.pop_back();

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, incompleteMessage(expectedPrintedData))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the msg_start state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1); // Invalid start char should be there
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], Gateway::XBeeController::START_CHARACTER);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0); // No checksum yet
    CHECK_EQUAL(m_uut->m_payload, ""); // No payload yet
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
}

TEST(XBeeControllerTest, incompleteMessageGotLength1State) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        Gateway::XBeeController::START_CHARACTER // Premature start char
    };

    // Get the expected data to be passed through
    std::vector<std::uint8_t> expectedPrintedData = data;
    expectedPrintedData.pop_back();

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, incompleteMessage(expectedPrintedData))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the msg_start state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1); // Invalid start char should be there
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], Gateway::XBeeController::START_CHARACTER);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0); // No checksum yet
    CHECK_EQUAL(m_uut->m_payload, ""); // No payload yet
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
}

TEST(XBeeControllerTest, incompleteMessageGotLength2State) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        0x14,
        Gateway::XBeeController::START_CHARACTER // Premature start char
    };

    // Get the expected data to be passed through
    std::vector<std::uint8_t> expectedPrintedData = data;
    expectedPrintedData.pop_back();

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, incompleteMessage(expectedPrintedData))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the msg_start state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1); // Invalid start char should be there
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], Gateway::XBeeController::START_CHARACTER);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0); // No checksum yet
    CHECK_EQUAL(m_uut->m_payload, ""); // No payload yet
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
}

TEST(XBeeControllerTest, incompleteMessageIgnoreOptionsState) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        0x14,
        0x10,           // Frame type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        Gateway::XBeeController::START_CHARACTER // Premature start char
    };

    // Get the expected data to be passed through
    std::vector<std::uint8_t> expectedPrintedData = data;
    expectedPrintedData.pop_back();

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, incompleteMessage(expectedPrintedData))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the msg_start state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1); // Invalid start char should be there
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], Gateway::XBeeController::START_CHARACTER);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0); // No checksum yet
    CHECK_EQUAL(m_uut->m_payload, ""); // No payload yet
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
}

TEST(XBeeControllerTest, incompleteMessageParsePayloadState) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        0x14,
        0x10,           // Frame Type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        0xff,           // Reserved 1
        0xfe,           // Reserved 2
        0x02,           // Options
        'H',            // Payload
        'e',
        'l',
        'l',
        'o',
        ' ',
        Gateway::XBeeController::START_CHARACTER // Premature start char
    };

    // Get the expected data to be passed through
    std::vector<std::uint8_t> expectedPrintedData = data;
    expectedPrintedData.pop_back();

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, incompleteMessage(expectedPrintedData))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the msg_start state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1); // Invalid start char should be there
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], Gateway::XBeeController::START_CHARACTER);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0); // No checksum yet
    CHECK_EQUAL(m_uut->m_payload, ""); // No payload yet
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
}

TEST(XBeeControllerTest, incompleteMessageCheckCheckSumState) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        0x14,
        0x10,           // Frame Type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        0xff,           // Reserved 1
        0xfe,           // Reserved 2
        0x02,           // Options
        'H',            // Payload
        'e',
        'l',
        'l',
        'o',
        ' ',
        Gateway::XBeeController::START_CHARACTER // Premature start char
    };

    std::vector<std::uint8_t> expectedPrintedData = data;
    expectedPrintedData.pop_back();

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, incompleteMessage(expectedPrintedData))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the msg_start state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 1); // Invalid start char should be there
    CHECK_EQUAL(m_uut->m_bytesProcessed[0], Gateway::XBeeController::START_CHARACTER);
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0); // No checksum yet
    CHECK_EQUAL(m_uut->m_payload, ""); // No payload yet
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::MSG_START);
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

TEST(XBeeControllerTest, nonVerboseSuccessTest) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        0x14,
        0x10,           // Frame Type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        0xff,           // Reserved 1
        0xfe,           // Reserved 2
        0x02,           // Options
        'H',            // Payload
        'e',
        'l',
        'l',
        'o',
        ' ',
        ':',
        ')',
        0x79
    };

    // When callback is called, kill the thread.
    auto killFunc = [&](){m_uut->kill(true);};
    EXPECT_CALL(*m_callbacks, successfulParse("Hello :)"))
        .WillOnce(testing::InvokeWithoutArgs(killFunc));

    m_uut->start();
    m_uut->addData(data);
    m_uut->join();

    // Everything should returm back to the startup state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 0); 
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
    CHECK_EQUAL(m_uut->m_payload, "");
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
}

TEST(XBeeControllerTest, badStateTest) {
    std::vector<std::uint8_t> data = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,
        0x14,
        0x10,           // Frame Type
        0x01,           // Frame ID
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x00,           // Address 6
        0xff,           // Address 7
        0xff,           // Reserved 1
        0xfe,           // Reserved 2
        0x02            // Recv Options
    };
    m_uut->m_bytesProcessed = data;
    m_uut->m_currentState = static_cast<Gateway::XBeeController::State>(-1);

    EXPECT_CALL(*m_callbacks, badState(data));

    m_uut->handleData();
    
    // Everything should returm back to the startup state
    CHECK_EQUAL(m_uut->m_dataLength, 0);
    CHECK_EQUAL(m_uut->m_bytesProcessed.size(), 0); 
    CHECK_EQUAL(m_uut->m_checkSumTotal, 0);
    CHECK_EQUAL(m_uut->m_payload, "");
    CHECK_EQUAL(m_uut->m_currentState, Gateway::XBeeController::State::STARTUP);
}

