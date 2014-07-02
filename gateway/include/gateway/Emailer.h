#ifndef EMAILER_H_
#define EMAILER_H_

#include <map>
#include <string>

#include "gateway/EmailerInterface.h"

namespace Gateway {

/**
 * \class Emailer
 * \brief Actually sends the email.  In this case, it opens a subprocess and calls curl.
 */
class Emailer : public EmailerInterface {
    public:
        /**
         * \param addresses The KEY in the map is the address, the VALUE 
         *                  is the name
         */
        Emailer(const std::map <std::string, std::string> &addresses,
                const std::string &subject, const std::string &message);

        ~Emailer();

        /**
         * \throws std::runtime_error if command cannot be executed
         * \return string from the output of the call to curl.
         */
        const std::string send() override;

    private:
        Emailer() = delete;

        const std::string getCurlCommand() const;

        std::map <std::string, std::string> m_addresses;
        std::string m_subject;
        std::string m_message;
};

}

#endif
