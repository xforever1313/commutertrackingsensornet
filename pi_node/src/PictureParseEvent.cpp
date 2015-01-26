#include "pi_node/PictureParseEvent.h"

namespace PiNode {

PictureParseEvent::PictureParseEvent(const std::string &pictureLocation,
                                     Common::EventExecutorInterface *eventExecutor) :
    m_pictureLocation(pictureLocation),
    m_eventExecutor(eventExecutor)
{
    //ctor
}

PictureParseEvent::~PictureParseEvent()
{
    //dtor
}

void PictureParseEvent::execute() {

}

}
