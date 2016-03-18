#include "threadpool.h"

namespace fluid
{

    bool ThreadPool::init(int threadNum) {
        bool ret = true;
        for(int i = 0; i < threadNum; i++) {
            Thread *thread = factory.makeThread();
            if (thread != NULL && thread->start()) {
                threads.push_back(thread);
            } else
                ret = false;
        }
        return ret;
    }

    void ThreadPool::release() {
        for(int i = 0; i < threads.size(); i++) {
            Thread* thread = threads[i];
            if(thread != NULL && thread->isJoinable()) {
                thread->final();
                thread->join();
                delete thread;
            }
        }
    }

    void ThreadPool::finalAll() {
        for(int i = 0; i < threads.size(); i++) {
            Thread* thread = threads[i];
            if (thread != NULL)
                thread->final();
        }
    }

    void ThreadPool::joinAll() {
        for(int i = 0; i < threads.size(); i++) {
            Thread* thread = threads[i];
            if(thread != NULL && thread->isJoinable()) {
                thread->join();
                delete thread;
            }
        }
    }

}

