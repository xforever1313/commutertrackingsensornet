#include "gateway/Gateway.h"
#include "EventExecutor.h"
#include "io/InputReader.h"

namespace Gateway {

Gateway::Gateway() :
    m_eventExecutor(new Common::EventExecutor),
    m_input(&std::cin),
    m_output(&Common::IO::ConsoleLogger::out)
{
}

Gateway::~Gateway()
{
    delete m_eventExecutor;
}

void Gateway::start() {
    std::string input = "";
    std::string promptMessage = "\nEnter a number:\n\t0.  Exit\n>";

    while (input != "0"){
        input = Common::IO::InputReader::PromptForInput(promptMessage, *m_input, *m_output);
    }
}

}
