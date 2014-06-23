#ifndef EMAILER_INTERFACE_H
#define EMAILER_INTERFACE_H_

#include <string>

namespace Gateway {

class EmailerInterface {
    public:
        virtual ~EmailerInterface() {}

        virtual std::string send() = 0;
};

}

#endif
