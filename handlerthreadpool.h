#ifndef __HANDLER_THREAD_POOL_H__
#define __HANDLER_THREAD_POOL_H__

#include "handlerthreadfactory.h"
#include "message.h"
#include <map>
#include "threadpool.h"
#include "singleton.h"

namespace fluid
{

class HandlerThreadPool : public ThreadPool, public HandlerThread
{
    private:
        HandlerThreadPool* next;
        HandlerThreadPool* prev;
        
        MsgQueue<std::pair<Channel*,Message*> > outQueue;
    protected:

        virtual HandlerThread* schedule(Channel* channel, Message *msg);
        
        std::pair<Channel*, Message*>* forward(std::pair<Channel*, Message*>* channelMsg);
        
        std::pair<Channel*, Message*>* backward(std::pair<Channel*, Message*>* channelMsg);

    public:
        HandlerThreadPool(QueueHandlerThreadFactory& factory):ThreadPool(factory),next(NULL),prev(NULL) {
            
        }
        /*
        void setNextPool(HandlerThreadPool* pool) {
            if (next == pool)
                return;
            insertPrevTo(pool);
        }
        
        void setPrevPool(HandlerThreadPool* pool) {
            if (prev == pool)
                return;
            insertNextTo(pool);
        }
        */
        HandlerThreadPool* getNextPool() {
            return next;
        }
        
        HandlerThreadPool* getPrevPool() {
            return prev;
        }
        
        void insertNextTo(HandlerThreadPool* pool) {
            if (!pool)
                return;
            this->next = pool->next;
            this->prev = pool;
            if (pool->next)
                pool->next->prev = this;
            pool->next = this;
        }
        
        void insertPrevTo(HandlerThreadPool* pool) {
            if (!pool)
                return;
            this->next = pool;
            this->prev = pool->prev;
            if (pool->prev)
                pool->prev->next = this;
            pool->prev = this;
        }

        bool init(int threadNum);

        virtual void onChannelClose(Channel* channel){}

        virtual void onChannelCreate(Channel* channel){}
        
        void forwardMessage(std::pair<Channel*, Message*>* channelMsg);
        
        void forwardMessage(Channel* channel, Message* msg) {
            forwardMessage(new std::pair<Channel*, Message*>(channel, msg));
        }
        
        void backwardMessage(std::pair<Channel*, Message*>* channelMsg);
        
        void backwardMessage(Channel* channel, Message* msg) {
            backwardMessage(new std::pair<Channel*, Message*>(channel, msg));
        }
        
        /**
         * 将输出消息发送到网络,需注意多线程问题
         */
        void flushOutMsg();

};

#if 0
/**
 * 单次分配线程池，即针对每个channel只分配一次线程，后续消息由相同线程处理
 */
class OnceAllocThreadPool : public HandlerThreadPool
{
private:
    std::map<int, HandlerThread*> channel2thread;
protected:
    HandlerThread* schedule(Channel* channel, Message *msg) {
        std::map<int,HandlerThread*>::iterator it = channel2thread.find(channel->id());
        if (it != channel2thread.end())
            return it->second;
        else {
            HandlerThread* thread = HandlerThreadPool::schedule(channel, msg);
            if (thread != NULL)
                channel2thread.insert(std::make_pair(channel->id(), thread));
            return thread;
        }
    }

    virtual void onChannelClose(Channel* channel){
        channel2thread.erase(channel->id());
    }

public:
    OnceAllocThreadPool(QueueHandlerThreadFactory& factory):HandlerThreadPool(factory) {
        
    }

};
#endif

class RecycleThreadPool : public HandlerThreadPool {
    public:
        class RecycleHandler : public Handler{
            public:
                Message* process(Channel* channel, Message* msg){
                    return NULL;
                }
                void onCreate(Channel* channel, ChannelEvent* event) {
                    
                }
                void onClose(Channel* channel, ChannelEvent* event) {
                    delete channel;
                }
        };
        class RecycleThreadFactory : public QueueHandlerThreadFactory,public Singleton<RecycleThreadFactory> {
            private:
                RecycleHandler handler;
            public:
                QueueHandlerThread* makeQueueHandlerThread() {
                    QueueHandlerThread* ret = new QueueHandlerThread(false);
                    if (ret != NULL)
                        ret->setForwardHandler(&handler);
                }
        };
        RecycleThreadPool():HandlerThreadPool(RecycleThreadFactory::getMe()) {
        
        }
};

}

#endif
