#ifndef ERROR_NUMBERS_H_
#define ERROR_NUMBERS_H_

namespace Gateway {

/**
 * \enum ErrorNumbers
 * \brief These are the same error numbers that live in the database.
 */
enum ErrorNumber {
    FIRST = 0,          ///< Not an error number, only used for error checking
    OKAY = 1,           ///< No error
    TEST_ERROR = 2,     ///< Used by the debug console, so we can see if this thing works or not.
    END                 ///< Not an actual error number, only used for error checking
};

}

#endif

