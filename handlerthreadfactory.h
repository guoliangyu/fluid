#ifndef __HANDLER_THREAD_FACTORY_H__
#define __HANDLER_THREAD_FACTORY_H__

#include "handlerthread.h"
#include "threadpool.h"

namespace fluid 
{

class HandlerThreadFactory : public ThreadFactory
{
    public:
        virtual HandlerThread* makeHandlerThread() = 0;

        virtual Thread* makeThread() {
            return (Thread*)makeHandlerThread();
        }
};

}

#endif
