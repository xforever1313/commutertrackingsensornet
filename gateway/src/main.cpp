#include <iostream>

#include "gateway/GatewayDateVersion.h"

int main() {
    std::cout << CTSN::Gateway::DateVersion::getVersionNumber() << std::endl;
    return 0;
}
