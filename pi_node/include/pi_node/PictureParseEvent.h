#ifndef PICTURE_PARSE_EVENT_H_
#define PICTURE_PARSE_EVENT_H_

#include <string>

#include "EventExecutorInterface.h"
#include "EventInterface.h"

namespace PiNode {

class PictureParseEvent : public Common::EventInterface {
    public:
        PictureParseEvent(const std::string &pictureLocation,
                          Common::EventExecutorInterface *eventExecutor);

        ~PictureParseEvent();

        void execute() override;

    private:
        const std::string m_pictureLocation;
        Common::EventExecutorInterface *m_eventExecutor;
};

}

#endif // PICTURE_PARSE_EVENT_H_
