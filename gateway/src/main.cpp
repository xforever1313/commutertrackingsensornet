#include "gateway/Gateway.h"
#include "io/ConsoleLogger.h"
#include "io/InputReader.h"

int main() {
    Common::IO::ConsoleLogger::out.writeLine("Welcome to the CTSN Gateway Node!");
    Gateway::Gateway gateway;
    gateway.start();
}
