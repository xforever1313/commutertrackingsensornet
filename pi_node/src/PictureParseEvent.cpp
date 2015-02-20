#include <cstdio>
#include <string>

#include "EventExecutorInterface.h"
#include "io/ConsoleLogger.h"
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
    Common::IO::ConsoleLogger::out.writeLineWithTimeStamp("Got picture: " + m_pictureLocation);
    if (m_removePicture) {
        Common::IO::ConsoleLogger::out.writeLineWithTimeStamp("Removing picture: " + m_pictureLocation);
        remove(m_pictureLocation.c_str());
    }
}

}
