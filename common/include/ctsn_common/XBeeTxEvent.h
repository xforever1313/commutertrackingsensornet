#ifndef XBEE_TX_EVENT_H_
#define XBEE_TX_EVENT_H_

#include <cstdint>
#include <string>
#include <vector>

#include "EventInterface.h"
#include "ctsn_common/Node.h"
#include "ctsn_common/UartInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"

namespace CTSNCommon {

class XBeeTxEvent : public Common::EventInterface {
    public:
        /**
         * \warning any message more than 2^16 in length will be truncated to 2^16
         */
        XBeeTxEvent(const Node &node,
                    const std::string &message, UartInterface *uart,
                    Common::IO::LoggerBase &logger = Common::IO::ConsoleLogger::err);

        ~XBeeTxEvent();

        void execute() override;

    private:
        void addStartChar();
        void addLength();
        void addFrameInfo();
        void addAddress();
        void addReservedBytes();
        void addTransmitOptions();
        void addPayload();
        void addChecksum();

        /**
         * \return true if the character needs to be escaped
         */
        bool isEscapedCharacter(uint8_t c) const;

        Node m_node;
        std::string m_message;
        UartInterface *m_uart;
        Common::IO::LoggerBase &m_errLogger;
        std::vector<std::uint8_t> m_packet;
        std::uint8_t m_checksum;
};

}

#endif

