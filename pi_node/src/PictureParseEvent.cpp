#include <opencv/cvaux.h>
#include <opencv/highgui.h>

#include <map>
#include <memory>
#include <string>

#include "ctsn_common/NodeContainerInterface.h"
#include "ctsn_common/SettingsParser.h"
#include "ctsn_common/XBeeHTTPTxEvent.h"
#include "ctsn_common/UartInterface.h"
#include "CTSNSharedGlobals.py"
#include "EventExecutorInterface.h"
#include "io/ConsoleLogger.h"
#include "pi_node/PictureParseEvent.h"

namespace PiNode {

PictureParseEvent::PictureParseEvent(CTSNCommon::NodeContainerInterface *nodes,
                                     CTSNCommon::UartInterface *uart,
                                     const std::string &pictureLocation,
                                     bool removePicture,
                                     Common::EventExecutorInterface *eventExecutor,
                                     Common::IO::LoggerBase &errLogger/* = Common::IO::ConsoleLogger::err*/) :
    m_nodes(nodes),
    m_uart(uart),
    m_pictureLocation(pictureLocation),
    m_removePicture(removePicture),
    m_eventExecutor(eventExecutor),
    m_errLogger(errLogger)
{
    m_hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}

PictureParseEvent::~PictureParseEvent() {
    if (m_removePicture) {
        remove(m_pictureLocation.c_str());
    }
}

std::vector<cv::Rect> PictureParseEvent::parsePicture() {

    cv::Mat img = cv::imread(m_pictureLocation.c_str());
    if (!img.data) {
        throw std::runtime_error("Could not open" + m_pictureLocation);
    }

    std::vector<cv::Rect> found;
    m_hog.detectMultiScale(
        img,
        found,
        0.0,
        cv::Size(8, 8),
        cv::Size(32, 32),
        1.05,
        2
    );
    return found;
}

int PictureParseEvent::getNumObjects() {
    // TODO: Modify this method to get better results.
    // Ideas include not getting things too close to the edge,
    // if something repeats n number of times, throw it out,
    // etc.

    const std::vector<cv::Rect> rect = parsePicture();
    return rect.size();
}

void PictureParseEvent::execute() {
    try {
        int numObjects = getNumObjects();

        for (int i = 0; i < numObjects; ++i) {
            std::map<std::string, std::string> httpData = {
                {"node", std::to_string(m_nodes->getCurrentNode().getID())},
                {"data", "2"} // 2 is pedestrian
            };
            std::shared_ptr<CTSNCommon::XBeeHTTPTxEvent> event (
                new CTSNCommon::XBeeHTTPTxEvent(m_nodes->getCurrentNode(), httpData, DATA_RESULT_URI, m_uart)
            );
            m_eventExecutor->addEvent(event);
        }
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
    }
}

}
