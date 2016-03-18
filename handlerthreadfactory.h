#ifndef __HANDLER_THREAD_FACTORY_H__
#define __HANDLER_THREAD_FACTORY_H__

#include "handlerthread.h"
#include "threadpool.h"
#include "queue_handler_thread.h"

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

class QueueHandlerThreadFactory : public HandlerThreadFactory
{
public:
    virtual QueueHandlerThread* makeQueueHandlerThread() = 0;
    
    HandlerThread* makeHandlerThread() {
        return (HandlerThread*)makeQueueHandlerThread();
    }
};

}

#endif
