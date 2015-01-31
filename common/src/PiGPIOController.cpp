#include <fstream>
#include <map>
#include <mutex>
#include <stdexcept>

#include "ctsn_common/PiGPIOController.h"
namespace CTSNCommon {

const std::string PiGPIOController::FILE_IO_ERROR = "Could not open file: ";
const std::string PiGPIOController::CONVERSION_ERROR = "Got bad value from file: ";std::mutex MUTEX_4;

std::mutex PiGPIOController::MUTEX_17;
std::mutex PiGPIOController::MUTEX_27;
std::mutex PiGPIOController::MUTEX_22;
std::mutex PiGPIOController::MUTEX_5;
std::mutex PiGPIOController::MUTEX_6;
std::mutex PiGPIOController::MUTEX_13;
std::mutex PiGPIOController::MUTEX_19;
std::mutex PiGPIOController::MUTEX_26;
std::mutex PiGPIOController::MUTEX_18;
std::mutex PiGPIOController::MUTEX_23;
std::mutex PiGPIOController::MUTEX_24;
std::mutex PiGPIOController::MUTEX_25;
std::mutex PiGPIOController::MUTEX_12;
std::mutex PiGPIOController::MUTEX_16;
std::mutex PiGPIOController::MUTEX_20;
std::mutex PiGPIOController::MUTEX_21;

PiGPIOController &PiGPIOController::getInstance() {
    static PiGPIOController instance;
    return instance;
}

PiGPIOController::PiGPIOController() :
    m_fileLocks({{17, MUTEX_17},
                 {27, MUTEX_27},
                 {22, MUTEX_22},
                 {5, MUTEX_5},
                 {6, MUTEX_6},
                 {13, MUTEX_13},
                 {19, MUTEX_19},
                 {26, MUTEX_26},
                 {18, MUTEX_18},
                 {24, MUTEX_24},
                 {25, MUTEX_25},
                 {12, MUTEX_12},
                 {16, MUTEX_16},
                 {20, MUTEX_20},
                 {21, MUTEX_21}})
{
}

PiGPIOController::~PiGPIOController() {

}

void PiGPIOController::set(int newOutput, unsigned int pinNumber) {
    std::lock_guard<std::mutex> (m_fileLocks.at(pinNumber));

    std::string file = "/sys/class/gpio/gpio" + std::to_string(pinNumber) + "/value";

    std::ofstream outFile(file);

    if (outFile.is_open()) {
        outFile << newOutput;
        outFile.close();
    }
    else {
        throw std::runtime_error(FILE_IO_ERROR + file);
    }
}

int PiGPIOController::get(unsigned int pinNumber) {
    int ret = 0;
    std::lock_guard<std::mutex> (m_fileLocks.at(pinNumber));

    std::string file = "/sys/class/gpio/gpio" + std::to_string(pinNumber) + "/value";

    std::ifstream inFile(file);

    if (inFile.is_open()) {
        inFile >> ret;

        if (inFile.fail()) {
            throw std::runtime_error(CONVERSION_ERROR + file);
        }

        inFile.close();
    }
    else {
        throw std::runtime_error(FILE_IO_ERROR + file);
    }

    return ret;
}

}
