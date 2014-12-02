#ifndef PIC_SEND_EVENT_H
#define PIC_SEND_EVENT_H

#include <string>

#include "ctsn_common/UartInterface.h"
#include "EventInterface.h"

namespace PiNode {

class PicSendEvent : public Common::EventInterface {
    public:
        PicSendEvent(const std::string &pictureFile,
                     CTSNCommon::UartInterface *uart);

        ~PicSendEvent();

        void execute() override;
    private:
        const std::string m_pictureFile;
        CTSNCommon::UartInterface *m_uart;

};

}

#endif

