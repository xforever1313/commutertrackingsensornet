#ifndef ERROR_NUMBERS_H_
#define ERROR_NUMBERS_H_

#include <string>

namespace Gateway {

/**
 * \enum ErrorNumbers
 * \brief These are the same error numbers that live in the database.
 */
enum ErrorNumber {
    FIRST = 0,                          ///< Not an error number, only used for error checking
    OKAY = 1,                           ///< No error
    TEST_ERROR = 2,                     ///< Used by the debug console, so we can see if this thing works or not.
    WEBSITE_ENTER_MAINTENANCE = 3,      ///< Website is entering maintenance mode.
    WEBSITE_EXIT_MAINTENANCE = 4,       ///< Website exiting maintenance mode.
    NODE_IS_NOW_GOOD = 5,               ///< Node had a bad state, but is now good.
    NODE_HAS_LOW_BATTERY = 6,           ///< Node has a low battery
    NODE_HAS_CRITICAL_BATTERY = 7,      ///< Node has a critical battery
    NODE_HAS_UNKNOWN_STATUS = 8,        ///< Node now has an unknown status.
    NODE_HAS_GONE_OFFLINE = 9,          ///< Node has purposefully been taken offline.
    END                                 ///< Not an actual error number, only used for error checking
};

/**
 * \brief Class that deals with functions that deal with error messages
 */
class ErrorMessage {
    public:
        /**
         * \brief Converts the given string to the message error number 
         * \throws std::out_of_range if the message is not a valid message number
         * \throws std::invalid_arugment if the given string is not an int
         */
        static ErrorNumber convertStringToMessage(const std::string &nodeString); 

    private:
        static const std::string INVALID_MESSAGE;
};

}

#endif

