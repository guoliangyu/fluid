#include "handlerthread.h"
#include "handlerthreadpool.h"

namespace fluid
{
    void HandlerThread::run() {
        while(!isFinal()){
            std::pair<Channel*, Message*> *m = msgQueue.get();
            if(NULL != m) {
                msgQueue.pop();
                handler.postMsg(m->first, m->second);
                if (nextPool != NULL) {
                    nextPool->post(m->first, m->second);
                } else {
                    delete m->second;
                    delete m;
                }
            }
        }
    }

    HandlerThread::~HandlerThread(){
        std::pair<Channel*, Message*> *m = msgQueue.get();
        while(NULL != m){ 
            msgQueue.pop();
            delete m->second;
            delete m;
        }   
    }   

    void HandlerThread::postMsg(Channel* channel, Message* msg) {
        msgQueue.push(new std::pair<Channel*, Message*>(channel, msg));
    } 

}

