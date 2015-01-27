#ifndef GPIO_CONTROLLER_INTERFACE_H_
#define GPIO_CONTROLLER_INTERFACE_H_

namespace CTSNCommon {

class GPIOControllerInterface {
    public:
        virtual ~GPIOControllerInterface() {}

        /**
         * \brief Sets the GPIO to the given value
         * \param newOutput new value to set the GPIO to.
         * \param pinNumber The pin number to read
         */
        void set(int newOuput, unsigned int pinNumber);

        /**
         * \brief Reads the value from the given GPIO pin.
         */
        int get(unsigned int pinNumber);
};

}

#endif
