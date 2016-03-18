#include "handlerthreadpool.h"
#include <utility> 
#include "channel.h"
#include "queue_handler_thread.h"

namespace fluid
{

    HandlerThread* HandlerThreadPool::schedule(Channel* channel, Message *msg) {
        if (threads.size() == 0)
            return NULL;
        int idx = channel->id() % threads.size();
        return (HandlerThread*)(threads[idx]);
    }
    
    void HandlerThreadPool::forwardMessage(std::pair<Channel*, Message*>* channelMsg) {
        if (channelMsg == NULL)
            return;
        QueueHandlerThread *thread = (QueueHandlerThread *)schedule(channelMsg->first, channelMsg->second);
        thread->forwardMessage(channelMsg);
    }
    
    void HandlerThreadPool::backwardMessage(std::pair<Channel*, Message*>* channelMsg) {
        if (channelMsg == NULL)
            return;
        QueueHandlerThread *thread = (QueueHandlerThread *)schedule(channelMsg->first, channelMsg->second);
        thread->backwardMessage(channelMsg);
    }
    
    std::pair<Channel*, Message*>* HandlerThreadPool::forward(std::pair<Channel*, Message*>* channelMsg) {
        if (channelMsg) {
            Message* msg = channelMsg->second;
            if (msg && msg->type() == MSG_TYPE_EVENT) {
                ChannelEvent *event = (ChannelEvent*)msg;
                if(event->isEvent(CHANNEL_EVENT_OPEN))
                    this->onChannelCreate(channelMsg->first);
                else if (event->isEvent(CHANNEL_EVENT_CLOSE)) 
                    this->onChannelClose(channelMsg->first);
            }
        }
        std::pair<Channel*, Message*>* ret = HandlerThread::forward(channelMsg);
        if (ret != NULL && next != NULL)
            next->forwardMessage(channelMsg);
    }
    
    std::pair<Channel*, Message*>* HandlerThreadPool::backward(std::pair<Channel*, Message*>* channelMsg) {
        std::pair<Channel*, Message*>* ret = HandlerThread::backward(channelMsg);
        if (ret != NULL && prev != NULL)
            prev->backwardMessage(channelMsg);
    }
    
    bool HandlerThreadPool::init(int threadNum) {
        if(!ThreadPool::init(threadNum))
            return false;
        for(int i = 0; i < threads.size(); i++) {
            QueueHandlerThread *thread = (QueueHandlerThread *)threads[i];
            if (thread) {
                this->addForwardQueue(&(thread->inQueueAfter));
                this->addBackwardQueue(&(thread->outQueueAfter));
            }
        }
        this->start();
    }
    
    void HandlerThreadPool::flushOutMsg() {
        for (int i = 0; i < backwardQueues.size(); i++) {
            MsgQueue<std::pair<Channel*, Message*> >* queue = backwardQueues[i];
            if (queue) {
                std::pair<Channel*, Message*>* m = queue->get();
                if (m != NULL) {
                    queue->pop();
                    Channel* channel = m->first;
                    Message* msg = m->second;
                    if (channel != NULL && msg != NULL) {
                        if (msg->type() == MSG_TYPE_RAW) {
                            RawMessage* rawMsg = (RawMessage*)msg;
                            channel->send(rawMsg->getData(), rawMsg->size());
                        } else if (msg->type() == MSG_TYPE_USER) {
                            int len = msg->byteSize();
                            unsigned char* data = (unsigned char*)malloc(len);
                            if (data != NULL && msg->serialize(data, len) > 0) {
                                channel->send(data, len);
                            }
                        }
                    }
                }
            }
        }
    }

}

