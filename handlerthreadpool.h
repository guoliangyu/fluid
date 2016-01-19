#ifndef __HANDLER_THREAD_POOL_H__
#define __HANDLER_THREAD_POOL_H__

#include "handlerthreadfactory.h"
#include "message.h"
#include <map>
#include "threadpool.h"
#include "singleton.h"

namespace fluid
{

class HandlerThreadPool : public ThreadPool
{
    private:
        std::map<int, Thread*>::iterator iter;

        HandlerThreadPool* next;
        
    protected:

        virtual HandlerThread* schedule(Channel* channel, Message *msg) {
            if (threads.empty())
                return NULL;
            if(iter == threads.end())
                iter = threads.begin();
            return (HandlerThread*)(iter++->second);
        }

    public:
        HandlerThreadPool(HandlerThreadFactory& factory):ThreadPool(factory),next(NULL) {
            iter = threads.begin();
        }

        void post(Channel *channel,Message* msg);

        void setNextPool(HandlerThreadPool* pool) {
            next = pool;
        }

        HandlerThreadPool* getNextPool() {
            return next;
        }

        bool init(int threadNum);

};

class RecycleThreadPool : public HandlerThreadPool {
    public:
        class RecycleHandler : public Handler{
            public:
                bool process(Channel* channel, Message* msg){
                    return true;
                }
                void onCreate(Channel* channel, ChannelEvent* event) {
                    
                }
                void onClose(Channel* channel, ChannelEvent* event) {
                    delete channel;
                }
        };
        class RecycleThreadFactory : public HandlerThreadFactory,public Singleton<RecycleThreadFactory> {
            private:
                RecycleHandler handler;
            public:
                HandlerThread* makeHandlerThread() {
                    return new HandlerThread(handler);
                }
        };
        RecycleThreadPool():HandlerThreadPool(RecycleThreadFactory::getMe()) {
        
        }
};

}

#endif
