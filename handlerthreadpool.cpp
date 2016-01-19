#include "handlerthreadpool.h"
#include <utility> 
#include "channel.h"

namespace fluid
{
    void HandlerThreadPool::post(Channel *channel, Message* msg) {
        HandlerThread *thread = schedule(channel, msg);
        if (thread != NULL)
            thread->postMsg(channel, msg);
    }

    bool HandlerThreadPool::init(int threadNum) {
        if(!ThreadPool::init(threadNum))
            return false;
        for(std::map<int, Thread*>::iterator it = threads.begin();it!=threads.end();++it) {
            if (it->second) {
                HandlerThread *handlerThread = (HandlerThread *)it->second;
                handlerThread->setNextPool(next);
            }
        }
    }

}

