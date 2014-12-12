#ifndef DATA_RESULT_TYPES
#define DATA_RESULT_TYPES

#include <string>

namespace CTSNCommon {

/**
 * \enum DataResultType
 * \brief This enum has values that represent the different entites
 *        that the trail can detect.
 * \note if the database changes, so will this.
 * \todo make this similar to the nodecontainers so we dont need
 *       to update this every time we compile.
 */
enum DataResultType{
    BAD_TYPE = 0,
    UNKNOWN = 1,
    WALKER = 2,
    BIKER = 3,
    HORSE = 4,
    TOO_HIGH = 5
};

/**
 * \brief converts the given string to a result type
 * \throws std::invalid_argument if the string is not an int
 * \throws std::out_of_range if the converted string is 0 or greater or equal to TOO_HIGH
 */
const DataResultType convertStringToResultType(const std::string &s);

const std::string DATA_RESULT_BAD_DATA_TYPE_STRING = "Bad Data Result String - "; ///<Thrown when the passed in string isn't an int
const std::string DATA_RESULT_INVALID_DATA_TYPE = "Invalid Data Result Type - ";    ///<Thrown when the passed in string is not a correct datatype

}

#endif

