#ifndef SHUTDOWNINTERFACE_H_INCLUDED
#define SHUTDOWNINTERFACE_H_INCLUDED

namespace CTSNCommon {

class ShutdownInterface {
    public:
        virtual ~ShutdownInterface() {}

        virtual void shutdown() = 0;
};

}

#endif // SHUTDOWNINTERFACE_H_INCLUDED
