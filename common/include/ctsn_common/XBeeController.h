#ifndef XBEE_CONTROLLER_H_
#define XBEE_CONTROLLER_H_

#include <cstdint>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#include "ctsn_common/XBeeCallbackInterface.h"
#include "ctsn_common/XBeeConstants.h"
#include "ctsn_common/UartRecvCallbackInterface.h"
#include "SSemaphore.h"
#include "SThread.h"

namespace CTSNCommon {

/**
 * \class XBeeController
 * \brief Parses the xbee packet and calls the correct callback.
 */
class XBeeController : public UartRecvCallbackInterface, public OS::Runnable<XBeeController> {
    public:
        /**
         * \note thread will not start until start() is called.
         */
        XBeeController(XBeeCallbackInterface *callbacks);

        /**
         * \brief kills AND joins the thread.
         */
        ~XBeeController();

        void addData(const std::vector<std::uint8_t> &data) override;

        /**
         *  \param shutdownSemaphore Shut downs the wait semaphore.  If
         *                           the thread is waiting on the data semaphore,
         *                           and kill is called, then the thread will never
         *                           exit.  This is only a param so during unit testing
         *                           we can not shutdown the semaphore if we dont want to
         */
        void kill(bool shutdownSemaphore = true);
        bool isAlive();

        /**
         * \brief Thread loop
         */
        void run();

    private:
        enum State {
            STARTUP,
            MSG_START,
            GOT_LENGTH1,
            GOT_LENGTH2,
            PARSE_MODEM_STATUS,
            IGNORE_OPTIONS,
            IGNORE_TX_STATUS_OPTIONS,
            PARSE_PAYLOAD,
            TX_PARSE_TX_RETRY,
            TX_PARSE_STATUS,
            TX_PARSE_DISCOVERY,
            CHECK_CHECKSUM,
            INCOMPLETE_MESSAGE
        };

        static const uint8_t BYTES_TO_IGNORE; /// <Bytes to ignore during the ignore state
        static const uint8_t TX_BYTES_TO_IGNORE;

        /**
         * \brief Reads and handles one piece of data from the queue.
         */
        void handleData();
        void handleStartupState();
        void handleMessageStartState();
        void handleGotLength1State();
        void handleGotLength2State();
        void handleParseModemStatusState();
        void handleIgnoreOptionsState();
        void handleIgnoreTxStatusOptions();
        void handleParsePayloadState();
        void handleTxParseTxRetryState();
        void handleTxParseStatusState();
        void handleTxParseDiscoveryState();
        void handleCheckCheckSumState();
        void handleIncompleteMessage();
        void handleBadState();
        void handleSuccessfulParse();

        void reset();
        uint8_t getNextByte();

        std::queue<std::uint8_t> m_data;
        std::mutex m_queueMutex;
        OS::SSemaphore m_dataSemaphore;

        std::uint16_t m_dataLength;
        std::uint8_t m_checkSumTotal;
        std::vector<std::uint8_t> m_bytesProcessed;
        std::uint64_t m_nonEscapedBytesProcessed;
        std::uint64_t m_lengthCounter; /// <Keepss track of the number of bytes between the length and checksum.

        std::string m_payload;

        bool m_isAlive;
        std::mutex m_isAliveMutex;

        State m_currentState;

        XBeeCallbackInterface *m_callbacks;

        bool m_escapedCharacter;
        CTSNCommon::XBeeConstants::PacketFrame m_packetFrame;
        CTSNCommon::XBeeConstants::ModemStatus m_modemStatus;
        uint8_t m_transmitRetryCount;
        CTSNCommon::XBeeConstants::TxStatus m_txStatus;
        CTSNCommon::XBeeConstants::DiscoveryStatus m_discoveryStatus;
};

}

#endif

