#ifndef NODE_H_
#define NODE_H_

#include <string>

namespace Gateway {

class Node {
    public:
        /**
         * \brief Converts the given string to the node number 
         * \throws std::out_of_range if the node is not a valid node number
         * \throws std::invalid_arugment if the given string is not an int
         */
        static unsigned int convertStringToNodeNumber(const std::string &nodeString); 

    private:
        static unsigned int numberOfNodes;
        static const std::string INVALID_NODE_MESSAGE;
};

}

#endif

