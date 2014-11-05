#include <stdexcept>
#include <string>
#include "gateway/DataResultTypes.h"

namespace Gateway {

const DataResultType convertStringToResultType(const std::string &s) {
    size_t stringSize = 0;
    int parsedType = DataResultType::BAD_TYPE;
    try {
        parsedType = std::stoi(s, &stringSize);
    }
    catch(const std::invalid_argument &e) {
        throw std::invalid_argument(DATA_RESULT_BAD_DATA_TYPE_STRING + s);
    }

    if (stringSize != s.size()) {
        throw std::invalid_argument(DATA_RESULT_BAD_DATA_TYPE_STRING + s);
    }
    else if ((parsedType <= DataResultType::BAD_TYPE) ||
             (parsedType >= DataResultType::TOO_HIGH)) {
        throw std::out_of_range(DATA_RESULT_INVALID_DATA_TYPE + s);
    }

    return static_cast<DataResultType>(parsedType);
}

}

