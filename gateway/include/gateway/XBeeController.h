#ifndef XBEE_CONTROLLER_H_
#define XBEE_CONTROLLER_H_

#include <cstdint>
#include <vector>

#include "gateway/UartRecvCallbackInterface.h"

namespace Gateway {

class XBeeController : public UartRecvCallbackInterface {
    public:
        XBeeController();
        ~XBeeController();

        void addData(const std::vector<std::uint8_t> &data) override;

    private:
        std::vector<std::uint8_t> m_data;
};

}

#endif

