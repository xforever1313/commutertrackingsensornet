#include <string>

#include "EventExecutorInterface.h"
#include "pi_node/PictureParseEvent.h"

namespace PiNode {

PictureParseEvent::PictureParseEvent(const std::string &pictureLocation,
                                     bool removePicture,
                                     Common::EventExecutorInterface *eventExecutor) :
    m_pictureLocation(pictureLocation),
    m_removePicture(removePicture),
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
