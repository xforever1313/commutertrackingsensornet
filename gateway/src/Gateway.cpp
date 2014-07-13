#include <functional>
#include <iostream>

#include "EventExecutor.h"
#include "gateway/EmailEvent.h"
#include "gateway/Gateway.h"
#include "gateway/TextMessageEvent.h"
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

void Gateway::sendEmail() {
    std::map <std::string, std::string> addresses;
    do {
        std::string name = Common::IO::InputReader::PromptForInput("\nEnter Person's name (EOF to stop): ", *m_input, *m_output);
        if (!m_input->eof()) {
            std::string email = Common::IO::InputReader::PromptForInput("Enter Person's Email: ", *m_input, *m_output);

            if (!email.empty()) {
                addresses[email] = name;
            }
            else {
                m_output->writeLine("Can not have empty email address!");
            }
        }
    } while (!m_input->eof());

    m_input->clear();

    if (!addresses.empty()) {
        std::string subject = Common::IO::InputReader::PromptForInput("Enter Subject: ", *m_input, *m_output);
        std::string message = Common::IO::InputReader::PromptForInput("Enter Message: ", *m_input, *m_output);
        m_eventExecutor->addEvent(std::shared_ptr<EmailEvent>(new EmailEvent(addresses, subject, message)));
    }
    else {
        std::cout << "No addresses entered!" << std::endl;
    }
}

void Gateway::sendTextMessage() {
    std::map <std::string, TextMessageEvent::Provider> numbers;
    std::string providerString;
    for (auto &provider : TextMessageEvent::PROVIDER_NAMES) {
        if (provider.first != TextMessageEvent::Provider::UNKNOWN) {
            providerString += "\t" + std::to_string(provider.first) + " " + provider.second + "\n";
        }
    }

    do {
        std::string number = Common::IO::InputReader::PromptForInput("\nEnter Person's number (EOF to stop): ", *m_input, *m_output);
        if (!m_input->eof()) {
            std::string providerInput = Common::IO::InputReader::PromptForInput("Enter Provider: \n" + providerString, *m_input, *m_output);
            if (providerInput.size() > 1) {
                m_output->writeLine("Invalid input");
            }
            else {
                //Convert number to int.
                int provider = providerInput[0] - '0';
                if ((provider < 1) || (provider >= TextMessageEvent::Provider::UNKNOWN)) {
                    m_output->writeLine("Invalid provider");
                }
                else {
                    numbers[number] = static_cast<TextMessageEvent::Provider>(provider);
                }
            }
        }
    } while (!m_input->eof());

    m_input->clear();

    if (!numbers.empty()) {
        std::string subject = Common::IO::InputReader::PromptForInput("Enter Subject: ", *m_input, *m_output);
        std::string message = Common::IO::InputReader::PromptForInput("Enter Message: ", *m_input, *m_output);
        m_eventExecutor->addEvent(std::shared_ptr<TextMessageEvent>(new TextMessageEvent(numbers, subject, message)));
    }
    else {
        std::cout << "No numbers entered!" << std::endl;
    }
}

void Gateway::start() {

    try {
        try {
            m_uart->open("/dev/ttyAMA0");
            m_recvThread->start();
        }
        catch(const std::runtime_error &e) {
            m_output->writeLine(e.what());
        }

        std::string input = "";
        std::string promptMessage = "\nEnter a number:\n\t1.  Uart Tx\n\t2.  Send Email\n\t3.  Send Text Message\n\t0.  Exit\n>";

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
            else if ((input == "2") && !m_input->fail()) {
                sendEmail();
            }
            else if ((input == "3") && !m_input->fail()) {
                sendTextMessage();
            }
        }
    }
    catch (const std::runtime_error &e) {
        m_output->writeLine(e.what());
    }

    m_recvThread->kill();
    m_uart->close();
}

}
