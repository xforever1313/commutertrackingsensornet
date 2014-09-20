#include <stdexcept>
#include <string>

#include "gateway/ErrorNumbers.h"

namespace Gateway {

const std::string ErrorMessage::INVALID_MESSAGE = "Invalid Error Message";

ErrorNumber ErrorMessage::convertStringToMessage(const std::string &messageString) {
    unsigned int errorNumber = 0;
    try{
        size_t stringSize;
        errorNumber = std::stoi(messageString, &stringSize);
         // Ensure theres nothing left over from the messageString.
        if (stringSize != messageString.size()) {
            throw std::invalid_argument(INVALID_MESSAGE);
        }
        //Message must be valid
        else if (errorNumber <= ErrorNumber::FIRST) {
            throw std::out_of_range(INVALID_MESSAGE);
        }
        else if (errorNumber >= ErrorNumber::END) {
            throw std::out_of_range(INVALID_MESSAGE);
        }
    }
    catch (const std::invalid_argument &e) {
        throw std::invalid_argument(INVALID_MESSAGE);
    }
    return static_cast<ErrorNumber>(errorNumber);
}

}

