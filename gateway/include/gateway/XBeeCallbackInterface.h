#ifndef XBEE_CALLBACK_INTERFACE_H_
#define XBEE_CALLBACK_INTERFACE_H_

#include <cstdint>
#include <string>
#include <vector>

namespace Gateway {

class XBeeCallbackInterface {
    public:
        virtual ~XBeeCallbackInterface(){}

        /**
         * \brief called when the parse is successful
         * \param payLoad the string from the message
         */
        virtual void successfulParse(const std::string &payload) = 0;

        /**
         * \brief called if a message is incomplete, that is the start character appears 
         *        before it was expected.
         * \param badData The data that caused the bad message.
         */
        virtual void incompleteMessage(const std::vector<std::uint8_t> &badData) = 0;

        /**
         * \brief Called if the checksum does not match
         * \param badData The data that caused the bad message
         */
        virtual void badChecksum(const std::vector<std::uint8_t> &badData) = 0;
};

}

#endif

