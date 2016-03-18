#ifndef __HANDLER_THREAD_H__
#define __HANDLER_THREAD_H__

#include "msgqueue.h"
#include <list>
#include "message.h"
#include <utility>
#include "handler.h"
#include "thread.h"
#include "channel.h"
#include <vector>

namespace fluid
{

class HandlerThreadPool;

class HandlerThread : public Thread
{

    protected:
        Handler* forwardHandler;
        Handler* backwardHandler;
        std::vector<MsgQueue<std::pair<Channel*,Message*> >* > forwardQueues;
        std::vector<MsgQueue<std::pair<Channel*,Message*> >* > backwardQueues;
        
        virtual std::pair<Channel*,Message*>* forward(std::pair<Channel*,Message*>* channelMsg);
        
        virtual std::pair<Channel*,Message*>* backward(std::pair<Channel*,Message*>* channelMsg);
        
        
    public:
        HandlerThread(bool joinable=false):forwardHandler(NULL),backwardHandler(NULL),Thread("", joinable){
            
        }
        
        void addForwardQueue(MsgQueue<std::pair<Channel*,Message*> >* q) {
            if (q == NULL)
                return;
            forwardQueues.push_back(q);
        }
        
        void addBackwardQueue(MsgQueue<std::pair<Channel*,Message*> >* q) {
            if (q == NULL)
                return;
            backwardQueues.push_back(q);
        }
        
        void setForwardHandler(Handler* handler) {
            forwardHandler = handler;
        }
        
        void setBackwardHandler(Handler* handler) {
            backwardHandler = handler;
        }

        ~HandlerThread();

        void run();

};

}

#endif
