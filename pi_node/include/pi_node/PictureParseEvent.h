#ifndef PICTURE_PARSE_EVENT_H_
#define PICTURE_PARSE_EVENT_H_

#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <string>

#include "ctsn_common/NodeContainerInterface.h"
#include "EventExecutorInterface.h"
#include "EventInterface.h"
#include "io/ConsoleLogger.h"
#include "io/LoggerBase.h"
#include "ctsn_common/UartInterface.h"

namespace PiNode {

class PictureParseEvent : public Common::EventInterface {
    public:
        PictureParseEvent(CTSNCommon::NodeContainerInterface *nodes,
                          CTSNCommon::UartInterface *uart,
                          const std::string &pictureLocation,
                          bool removePicture,
                          Common::EventExecutorInterface *eventExecutor,
                          Common::IO::LoggerBase &errLogger = Common::IO::ConsoleLogger::err);

        ~PictureParseEvent();

        void execute() override;

    private:
        std::vector<cv::Rect> parsePicture();
        int getNumObjects();

        CTSNCommon::NodeContainerInterface *m_nodes;
        CTSNCommon::UartInterface *m_uart;
        const std::string m_pictureLocation;
        bool m_removePicture;
        Common::EventExecutorInterface *m_eventExecutor;
        cv::HOGDescriptor m_hog;
        Common::IO::LoggerBase &m_errLogger;
};

}

#endif // PICTURE_PARSE_EVENT_H_
