/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QueueHandlerThread.h
 * Author: netease
 *
 * Created on 2016年3月8日, 上午10:01
 */

#ifndef QUEUEHANDLERTHREAD_H
#define QUEUEHANDLERTHREAD_H

#include "handlerthread.h"
#include <glog/logging.h>

namespace fluid {

    class QueueHandlerThread : public HandlerThread {
        
        friend class HandlerThreadPool;

    protected:
        MsgQueue<std::pair<Channel*, Message*> > inQueue;
        MsgQueue<std::pair<Channel*, Message*> > inQueueAfter;
        MsgQueue<std::pair<Channel*, Message*> > outQueue;
        MsgQueue<std::pair<Channel*, Message*> > outQueueAfter;

    public:

        QueueHandlerThread(bool joinable = false) : HandlerThread(joinable) {
            this->addForwardQueue(&inQueue);
            this->addBackwardQueue(&outQueue);
        }

        ~QueueHandlerThread();

        void forwardMessage(Channel* channel, Message* m) {
            inQueue.push(new std::pair<Channel*, Message*>(channel, m));
        }

        void forwardMessage(std::pair<Channel*, Message*> *m) {
            inQueue.push(m);
        }

        void backwardMessage(std::pair<Channel*, Message*> *m) {
            outQueue.push(m);
        }

        void backwardMessage(Channel* channel, Message* m) {
            outQueue.push(new std::pair<Channel*, Message*>(channel, m));
        }

        std::pair<Channel*,Message*>* forward(std::pair<Channel*, Message*> *m) {
            std::pair<Channel*,Message*>* ret = HandlerThread::forward(m);
            if (ret != NULL)
                inQueueAfter.push(m);
            return ret;
        }

        std::pair<Channel*,Message*>* backward(std::pair<Channel*, Message*> *m) {
            std::pair<Channel*,Message*>* ret = HandlerThread::backward(m);
            if (ret != NULL){
                outQueueAfter.push(m);
            }
            return ret;
        }

    };

}

#endif /* QUEUEHANDLERTHREAD_H */

