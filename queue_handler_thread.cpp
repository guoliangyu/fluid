/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QueueHandlerThread.cpp
 * Author: netease
 * 
 * Created on 2016年3月8日, 上午10:01
 */

#include "queue_handler_thread.h"
#include "handlerthreadpool.h"

namespace fluid {

    QueueHandlerThread::~QueueHandlerThread() {
        std::pair<Channel*, Message*> *m = inQueue.get();
        while (NULL != m) {
            inQueue.pop();
            delete m->second;
            delete m;
        }
        m = inQueueAfter.get();
        while (NULL != m) {
            inQueueAfter.pop();
            delete m->second;
            delete m;
        }
        m = outQueue.get();
        while (NULL != m) {
            outQueue.pop();
            delete m->second;
            delete m;
        }
        m = outQueueAfter.get();
        while (NULL != m) {
            outQueueAfter.pop();
            delete m->second;
            delete m;
        }
    }

}
