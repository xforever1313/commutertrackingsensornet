#ifndef DATA_RESULT_TYPES
#define DATA_RESULT_TYPES

namespace Gateway {

/**
 * \note if the database changes, so will this.
 * \todo make this similar to the nodecontainers so we dont need 
 *       to update this every time we compile.
 */
enum DataResultType{
    BAD_TYPE = 0,
    UNKNOWN = 1,
    WALKER = 2,
    BIKER = 3,
    HORSE = 4
};

}

#endif

