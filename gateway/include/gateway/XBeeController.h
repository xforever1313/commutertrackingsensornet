#ifndef XBEE_CONTROLLER_H_
#define XBEE_CONTROLLER_H_

#include <cstdint>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#include "gateway/UartRecvCallbackInterface.h"
#include "SSemaphore.h"
#include "SThread.h"

namespace Gateway {

class XBeeController : public UartRecvCallbackInterface, public OS::SThread {
    public:
        /**
         * \note thread will not start until start() is called.
         */
        XBeeController();

        /**
         * \brief kills AND joins the thread.
         */
        ~XBeeController();

        void addData(const std::vector<std::uint8_t> &data) override;

        void kill();
        bool isAlive();

    private:
        enum State {
            STARTUP,
            MSG_START,
            GOT_LENGTH1,
            GOT_LENGTH2,
            IGNORE_OPTIONS,
            PARSE_MESSAGE,
            CHECK_CHECKSUM,
            SEND_COMMAND,
            BAD_CHECKSUM,
            INCOMPLETE_MESSAGE
        };

        static const uint8_t START_CHARACTER;

        /**
         * \brief Thread loop
         */
        void run() override;

        /**
         * \brief Reads and handles one piece of data from the queue.
         */
        void handleData();
        void handleStartupState();

        std::queue<std::uint8_t> m_data;
        OS::SSemaphore m_dataSemaphore;

        bool m_isAlive;
        OS::SMutex m_isAliveMutex;

        State m_currentState;
};

}

#endif

