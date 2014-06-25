#include <stdexcept>
#include <string>
#include <map>

#include "gateway/Emailer.h"
#include "gateway/EmailEvent.h"

namespace Gateway {

EmailEvent::EmailEvent(const std::map <std::string, std::string> &addresses,
                       const std::string &subject, const std::string &message,
                       Common::IO::LoggerBase &outLogger /*= Common::IO::ConsoleLogger::out*/,
                       Common::IO::LoggerBase &errLogger /*= Common::IO::ConsoleLogger::err*/) :
    m_emailer(new Emailer(addresses, subject, message)),
    m_outLogger(outLogger),
    m_errLogger(errLogger)
{

}

EmailEvent::~EmailEvent() {
    delete m_emailer;
}

void EmailEvent::execute() {
    try {
        std::string curlOutput = m_emailer->send();
        m_outLogger.writeLine(curlOutput);
    }
    catch (const std::runtime_error &e) {
        m_errLogger.writeLine(e.what());
    }
}

}