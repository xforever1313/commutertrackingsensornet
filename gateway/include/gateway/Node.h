#ifndef NODE_H_
#define NODE_H_

#include <cstdint>

namespace Gateway {

class Node {
    public:
        /**
         * \enum NodeStatus
         * \brief These are the same node statuses that live in the database.
         *        If the database is updated, this will also need to be.
         */
        enum NodeStatus {
            FIRST = 0,              ///< Not an error number, only used for error checking
            OKAY = 1,               ///< No error
            ASLEEP = 2,             ///< Node is asleep purposefully
            OFFLINE = 3,            ///< Node is offline purposefully
            MAINTENANCE = 4,        ///< Node is offline for maintenance
            LOW_BATTERY = 5,        ///< Node's battery is gettting low
            BATTERY_CRiTICAL = 6,   ///< Node's battery is about to run out
            UNKNOWN = 7,            ///< Node is at an unknown state, we can not reach it.
            DOWN = 8,               ///< The node is just down.
            END                     ///< Not an error number, only used for error checking
        };

        Node (unsigned int id, uint64_t address,
              NodeStatus status = NodeStatus::UNKNOWN);
        ~Node();

        unsigned int getID() const;

        uint64_t getAddress() const;

        NodeStatus getStatus() const;

    private:
        unsigned int m_id;
        uint64_t m_address;
        NodeStatus m_status;
};

}

#endif

