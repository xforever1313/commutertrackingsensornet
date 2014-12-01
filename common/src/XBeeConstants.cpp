#include <cstdint>

#include "ctsn_common/XBeeConstants.h"

namespace CTSNCommon {

const uint8_t XBeeConstants::START_CHARACTER = '~';
const uint8_t XBeeConstants::ESCAPE_CHARACTER = 0x7D;
const uint8_t XBeeConstants::XON = 0x11;
const uint8_t XBeeConstants::XOFF = 0x13;
const uint8_t XBeeConstants::ESCAPE_XOR = 0x20;

}

