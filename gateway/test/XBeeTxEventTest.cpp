#include <cstdint>
#include <string>
#include <vector>

#include "UnitTest.h"

#include "gateway/XBeeController.h"
#include "gateway/XBeeTxEvent.h"
#include "io/StringLogger.h"
#include "MockUart.h"

TEST_GROUP(XBeeTxEventTest) {
    TEST_SETUP() {
        m_node = new Gateway::Node(1, 0x0000000000001001);
        m_message = "Hello! :)";
        m_logger = new Common::IO::StringLogger();
        m_mockUart = new testing::StrictMock<Gateway::MockUart>();
        m_uut = new Gateway::XBeeTxEvent(*m_node, m_message, m_mockUart, *m_logger);

        POINTERS_EQUAL(m_uut->m_uart, m_mockUart);
        CHECK_EQUAL(m_uut->m_node.getID(), m_node->getID());
        CHECK_EQUAL(m_uut->m_message, m_message);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_mockUart;
        delete m_logger;
        delete m_node;
    }

    std::string m_message;


    Gateway::Node *m_node;
    Common::IO::StringLogger *m_logger;
    testing::StrictMock<Gateway::MockUart> *m_mockUart;
    Gateway::XBeeTxEvent *m_uut;
};

TEST(XBeeTxEventTest, successTest) {
    std::vector<std::uint8_t> expectedData = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,           // Length 1
        0x17,           // Length 2
        0x10,           // Frame type
        0x01,           // Frame ID
        0x00,           // Address 0
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x10,           // Address 6
        0x01,           // Address 7
        0xFF,           // Reserved
        0xFE,           // Reserved
        0x00,           // Broadcast Radius
        0x00,           // Transmit Options
        static_cast<uint8_t>(m_message[0]),   // 0x48
        static_cast<uint8_t>(m_message[1]),   // 0x65
        static_cast<uint8_t>(m_message[2]),   // 0x6C
        static_cast<uint8_t>(m_message[3]),   // 0x6C
        static_cast<uint8_t>(m_message[4]),   // 0x6F
        static_cast<uint8_t>(m_message[5]),   // 0x21
        static_cast<uint8_t>(m_message[6]),   // 0x20
        static_cast<uint8_t>(m_message[7]),   // 0x3A
        static_cast<uint8_t>(m_message[8]),   // 0x29
        0x48            // Checksum
    };

    EXPECT_CALL(*m_mockUart, send(expectedData));

    m_uut->execute();

    CHECK_EQUAL(m_logger->getString(), "");
}

TEST(XBeeTxEventTest, errorTest) {
    std::vector<std::uint8_t> expectedData = {
        Gateway::XBeeController::START_CHARACTER,
        0x00,           // Length 1
        0x17,           // Length 2
        0x10,           // Frame type
        0x01,           // Frame ID
        0x00,           // Address 0
        0x00,           // Address 1
        0x00,           // Address 2
        0x00,           // Address 3
        0x00,           // Address 4
        0x00,           // Address 5
        0x10,           // Address 6
        0x01,           // Address 7
        0xFF,           // Reserved
        0xFE,           // Reserved
        0x00,           // Broadcast Radius
        0x00,           // Transmit Options
        static_cast<uint8_t>(m_message[0]),   // 0x48
        static_cast<uint8_t>(m_message[1]),   // 0x65
        static_cast<uint8_t>(m_message[2]),   // 0x6C
        static_cast<uint8_t>(m_message[3]),   // 0x6C
        static_cast<uint8_t>(m_message[4]),   // 0x6F
        static_cast<uint8_t>(m_message[5]),   // 0x21
        static_cast<uint8_t>(m_message[6]),   // 0x20
        static_cast<uint8_t>(m_message[7]),   // 0x3A
        static_cast<uint8_t>(m_message[8]),   // 0x29
        0x48            // Checksum
    };

    std::string expectedString = "ERROR!!!";

    EXPECT_CALL(*m_mockUart, send(expectedData))
        .WillOnce(testing::Throw(std::runtime_error(expectedString)));

    m_uut->execute();

    CHECK(m_logger->getString().find(expectedString) != std::string::npos);
}
