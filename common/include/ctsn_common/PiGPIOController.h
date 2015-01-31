#ifndef PIGPIOCONTROLLER_H
#define PIGPIOCONTROLLER_H

#include <map>
#include <mutex>

#include "ctsn_common/GPIOControllerInterface.h"

namespace CTSNCommon {

class PiGPIOController : public GPIOControllerInterface
{
    public:
        static PiGPIOController &getInstance();

        ~PiGPIOController();

        /**
         * \brief Sets the GPIO to the given value
         * \param newOutput new value to set the GPIO to.
         * \param pinNumber The pin number to read
         * \throw std::runtime_error if can not open file.
         * \throw std::out_of_range if pinNumber does not exist
         */
        void set(int newOuput, unsigned int pinNumber) override;

        /**
         * \brief Reads the value from the given GPIO pin.
         * \throw std::runtime_error if can not open file
         * \throw std::runtime_error if can not convert file contents to uint.
         * \throw std::out_of_range if pinNumber does not exist
         */
        int get(unsigned int pinNumber) override;

    private:
        static const std::string FILE_IO_ERROR;
        static const std::string CONVERSION_ERROR;

        static std::mutex MUTEX_4;
        static std::mutex MUTEX_17;
        static std::mutex MUTEX_27;
        static std::mutex MUTEX_22;
        static std::mutex MUTEX_5;
        static std::mutex MUTEX_6;
        static std::mutex MUTEX_13;
        static std::mutex MUTEX_19;
        static std::mutex MUTEX_26;
        static std::mutex MUTEX_18;
        static std::mutex MUTEX_23;
        static std::mutex MUTEX_24;
        static std::mutex MUTEX_25;
        static std::mutex MUTEX_12;
        static std::mutex MUTEX_16;
        static std::mutex MUTEX_20;
        static std::mutex MUTEX_21;

        PiGPIOController();

        PiGPIOController(const PiGPIOController&) = delete;
        PiGPIOController &operator=(const PiGPIOController&) = delete;

        std::map<unsigned int, std::mutex&> m_fileLocks;

};

}

#endif // PIGPIOCONTROLLER_H
