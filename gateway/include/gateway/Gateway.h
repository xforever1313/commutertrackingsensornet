#ifndef GATEWAY_H
#define GATEWAY_H

#include <istream>

#include "EventExecutorInterface.h"
#include "io/LoggerBase.h"

namespace Gateway {

class Gateway {
    public:
        Gateway();
        virtual ~Gateway();

        void start();
    private:
        Gateway(const Gateway &other) = delete;

        Common::EventExecutorInterface *m_eventExecutor;
        std::istream *m_input;
        Common::IO::LoggerBase *m_output;
};

}

#endif // GATEWAY_H
