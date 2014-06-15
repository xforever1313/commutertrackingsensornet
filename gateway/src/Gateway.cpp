#include <functional>
#include <iostream>

#include "EventExecutor.h"
#include "gateway/Gateway.h"
#include "gateway/Uart.h"
#include "gateway/UartRecvThread.h"
#include "gateway/UartTxEvent.h"
#include "io/InputReader.h"

namespace Gateway {

void Gateway::RxSignal(int status) {
    getInstance().m_recvThread->dataReady();
}

Gateway &Gateway::getInstance() {
    static Gateway g;
    return g;
}

Gateway::Gateway() :
    m_eventExecutor(new Common::EventExecutor),
    m_input(&std::cin),
    m_output(&Common::IO::ConsoleLogger::out),
    m_uart(new Uart(RxSignal)),
    m_recvThread(new UartRecvThread(m_uart))
{
}

Gateway::~Gateway() {
    delete m_recvThread;
    delete m_uart;
    delete m_eventExecutor;
}

void Gateway::start() {

    try {
        m_uart->open("/dev/ttyAMA0");
        m_recvThread->start();

        std::string input = "";
        std::string promptMessage = "\nEnter a number:\n\t1.  Uart Tx\n\t0.  Exit\n>";

        while (input != "0"){
            if (m_input->fail()) {
                m_input->clear();
                input = Common::IO::InputReader::PromptForInput("\n>", *m_input, *m_output);
            }
            else {
                input = Common::IO::InputReader::PromptForInput(promptMessage, *m_input, *m_output);
            }

            if ((input == "1") && !m_input->fail()) {
                std::string promptMessage2 = "\nEnter a message to send:\n>";
                std::string msg = Common::IO::InputReader::PromptForInput(promptMessage2, *m_input, *m_output);
                m_eventExecutor->addEvent(std::shared_ptr<Common::EventInterface>(new UartTxEvent(msg, m_uart)));
            }
        }
    }
    catch (const std::runtime_error &e) {
        m_output->writeLine(e.what());
    }
}

}
