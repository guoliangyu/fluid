#ifndef __HANDLER_THREAD_H__
#define __HANDLER_THREAD_H__

#include "msgqueue.h"
#include <list>
#include "message.h"
#include <utility>
#include "handler.h"
#include "thread.h"
#include "channel.h"

namespace fluid
{

class HandlerThreadPool;

class HandlerThread : public Thread
{
    friend class HandlerThreadPool;

    private:
        Handler& handler;
        MsgQueue<std::pair<Channel*, Message*> > msgQueue;
        HandlerThreadPool *nextPool;
        
        void setNextPool(HandlerThreadPool* pool) {
            nextPool = pool;
        }
        
    public:
        HandlerThread(Handler& h, bool joinable=false):handler(h),Thread("", joinable){
            nextPool = NULL;
        }

        ~HandlerThread();

        void postMsg(Channel* channel, Message* msg);

        void run();
};

}

#endif
