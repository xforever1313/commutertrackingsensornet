#ifndef PICTURE_PARSER_H_
#define PICTURE_PARSER_H_

#include <Poco/Net/HTTPServer.h>

#include "ctsn_common/ShutdownInterface.h"
#include "EventExecutorInterface.h"
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

        OS::SSemaphore m_shutdownSemaphore;

        Poco::Net::ServerSocket *m_socket;
        Poco::Net::HTTPServer *m_server;
};

}

#endif

