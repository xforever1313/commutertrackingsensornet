#ifndef NODE_H_
#define NODE_H_

#include <cstdint>

namespace Gateway {

class Node {
    public:
        Node (unsigned int id, uint64_t address);
        ~Node();

        unsigned int getID() const;

        uint64_t getAddress() const;

    private:
        unsigned int m_id;
        uint64_t m_address;
};

}

#endif

