#ifndef PICTURE_PARSER_H_
#define PICTURE_PARSER_H_

#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/HTTPPosterInterface.h"
#include "ctsn_common/ShutdownInterface.h"
#include "EventExecutorInterface.h"
#include "picture_parser/CVRunnerInterface.h"
#include "picture_parser/PictureContainer.h"
#include "picture_parser/PictureParseEvent.h"

#include "SSemaphore.h"

namespace PictureParser {

class PictureParser : public CTSNCommon::ShutdownInterface {
    public:
        static PictureParser &getInstance();

        ~PictureParser();

        void start();

    private:
        PictureParser();
        PictureParser(const PictureParser &other) = delete;

        void initHTTPServer();
        void shutdown() override;

        Common::EventExecutorInterface *m_eventExecutor;
        PictureContainer *m_pc;
        CVRunnerInterface *m_cvRunner;
        CTSNCommon::HTTPPosterInterface *m_httpPoster;

        OS::SSemaphore m_shutdownSemaphore;

        Poco::Net::ServerSocket *m_socket;
        Poco::Net::HTTPServer *m_server;
};

}

#endif

