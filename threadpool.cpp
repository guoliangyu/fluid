#include "threadpool.h"

namespace fluid
{

    bool ThreadPool::init(int threadNum) {
        bool ret = true;
        for(int i = 0; i < threadNum; i++) {
            Thread *thread = factory.makeThread();
            if (thread != NULL && thread->start()) {
                threads.insert(std::make_pair(thread->getId(), thread));
            } else
                ret = false;
        }
        return ret;
    }

    void ThreadPool::release() {
        for(std::map<int, Thread*>::iterator it = threads.begin(); it != threads.end(); ++it) {
            Thread* thread = it->second;
            if(thread != NULL && thread->isJoinable()) {
                thread->final();
                thread->join();
                delete thread;
                it->second = NULL;
            }
        }
    }

    void ThreadPool::finalAll() {
        for(std::map<int, Thread*>::iterator it = threads.begin(); it != threads.end(); ++it) {
            if (it->second)
                it->second->final();
        }
    }

    void ThreadPool::joinAll() {
        for(std::map<int, Thread*>::iterator it = threads.begin(); it != threads.end(); ++it) {
            Thread* thread = it->second;
            if(thread != NULL && thread->isJoinable()) {
                thread->join();
                delete thread;
            }
        }
    }

}

