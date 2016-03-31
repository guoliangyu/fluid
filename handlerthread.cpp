#include "handlerthread.h"
#include "handlerthreadpool.h"

namespace fluid
{
    void HandlerThread::run() {
        while(!isFinal()){
            bool idle = true;
            for(int i = 0; i < forwardQueues.size(); i++) {
                MsgQueue<std::pair<Channel*,Message*> >* queue = forwardQueues[i];
                if (queue) {
                    std::pair<Channel*,Message*>* m = queue->get();
                    if (m != NULL) {
                        queue->pop();
                        this->forward(m);
                        idle = false;
                    }
                }
            }
            for(int i = 0; i < backwardQueues.size(); i++) {
                MsgQueue<std::pair<Channel*,Message*> >* queue = backwardQueues[i];
                if (queue) {
                    std::pair<Channel*,Message*>* m = queue->get();
                    if (m != NULL) {
                        queue->pop();
                        this->backward(m);
                        idle = false;
                    }
                }
            }
            if (idle)
                msleep(1);
        }
    }
    
    std::pair<Channel*,Message*>* HandlerThread::forward(std::pair<Channel*,Message*>* channelMsg) {
        Message* msg = channelMsg->second;
        if (forwardHandler) {
            msg = forwardHandler->postMsg(channelMsg->first, channelMsg->second);
        }
        if (msg == NULL) {
            delete channelMsg->second;
            delete channelMsg;
            return NULL;
        } else if ( msg != channelMsg->second ) { 
            delete channelMsg->second;
            channelMsg->second = msg;
            return channelMsg;
        }
        return channelMsg;
    }
        
    std::pair<Channel*,Message*>* HandlerThread::backward(std::pair<Channel*,Message*>* channelMsg) {
        Message* msg = channelMsg->second;
        if (backwardHandler) {
            msg = backwardHandler->postMsg(channelMsg->first, channelMsg->second);
        }
        if (msg == NULL) {
            delete channelMsg->second;
            delete channelMsg;
            return NULL;
        } else if ( msg != channelMsg->second ) { 
            delete channelMsg->second;
            channelMsg->second = msg;
            return channelMsg;
        }
        return channelMsg;
    }

    HandlerThread::~HandlerThread(){
        
    }   

}

