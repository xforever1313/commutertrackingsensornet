#ifndef XBEE_CONTROLLER_H_
#define XBEE_CONTROLLER_H_

#include <cstdint>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#include "gateway/XBeeCallbackInterface.h"
#include "gateway/UartRecvCallbackInterface.h"
#include "SSemaphore.h"
#include "SThread.h"

namespace Gateway {

class XBeeController : public UartRecvCallbackInterface, public OS::SThread {
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

    private:
        enum State {
            STARTUP,
            MSG_START,
            GOT_LENGTH1,
            GOT_LENGTH2,
            IGNORE_OPTIONS,
            PARSE_PAYLOAD,
            CHECK_CHECKSUM,
            INCOMPLETE_MESSAGE
        };

        static const uint8_t BYTES_TO_IGNORE; /// <Bytes to ignore during the ignore state

        /**
         * \brief Thread loop
         */
        void run() override;

        /**
         * \brief Reads and handles one piece of data from the queue.
         */
        void handleData();
        void handleStartupState();
        void handleMessageStartState();
        void handleGotLength1State();
        void handleGotLength2State();
        void handleIgnoreOptionsState();
        void handleParsePayloadState();
        void handleCheckCheckSumState();
        void handleIncompleteMessage();
        void handleBadState();

        void reset();

        std::queue<std::uint8_t> m_data;
        OS::SSemaphore m_dataSemaphore;

        std::uint16_t m_dataLength;
        std::uint8_t m_checkSumTotal;
        std::vector<std::uint8_t> m_bytesProcessed;
        std::uint64_t m_nonEscapedBytesProcessed;
        std::uint64_t m_lengthCounter; /// <Keepss track of the number of bytes between the length and checksum.

        std::string m_payload;

        bool m_isAlive;
        OS::SMutex m_isAliveMutex;

        State m_currentState;

        XBeeCallbackInterface *m_callbacks;

        bool m_escapedCharacter;
};

}

#endif

