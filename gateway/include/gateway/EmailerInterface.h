#ifndef EMAILER_INTERFACE_H_
#define EMAILER_INTERFACE_H_

#include <string>

namespace Gateway {

class EmailerInterface {
    public:
        virtual ~EmailerInterface() {}

        virtual const std::string send() = 0;
};

}

#endif
