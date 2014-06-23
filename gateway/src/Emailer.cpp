#include <map>
#include <stdexcept>
#include <string>

#include "gateway/Emailer.h"

namespace Gateway {

const std::string Emailer::POST_URL = "https://api.mailgun.net/v2/seth.thenaterhood.com/messages";
const std::string Emailer::FROM = "CTSN <postmaster@seth.thenaterhood.com>";

Emailer::Emailer(const std::map <std::string, std::string> &addresses,
                 const std::string &subject, const std::string &message) :
    m_addresses(addresses),
    m_subject(subject),
    m_message(message)
{
}


Emailer::~Emailer() {
}

std::string Emailer::send() {
    return "";
}

}