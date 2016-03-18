/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoThread.h
 * Author: hzyuguoliang
 *
 * Created on 2016年3月7日, 下午8:14
 */

#ifndef DOTHREAD_H
#define DOTHREAD_H

#include "msgqueue.h"
#include "thread.h"

namespace fluid
{
    template<typename Req>
    class DoThread : public Thread {
    public:
        DoThread() : Thread("DoThread", false){
        }
        virtual ~DoThread() {
        }

        void run() {
            while (!isFinal()) {
                Req *m = reqQueue.get();
                if (NULL != m) {
                    reqQueue.pop();
                    process(m);
                    delete m;
                } else 
                    msleep(1);
            }
        }

        virtual void process(Req* req) = 0;

        void push(Req* req) {
            reqQueue.push(req);
        }
    protected:
        MsgQueue<Req> reqQueue;

    };
}
#endif /* DOTHREAD_H */

