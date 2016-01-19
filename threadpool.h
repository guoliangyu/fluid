#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <map>
#include "thread.h"
#include "threadfactory.h"

namespace fluid
{

class ThreadPool
{
    protected:
        std::map<int, Thread*> threads;
        ThreadFactory& factory;

    public:
        ThreadPool(ThreadFactory& f):factory(f)
        {
        }

        bool addThread(Thread* thread) {
            if(thread == NULL)
                return false;
            if (threads.find(thread->getId()) != threads.end())
                return false;
            threads.insert(std::make_pair(thread->getId(), thread));
            return true;
        }

        Thread* getThread(int id) {
            std::map<int, Thread*>::iterator it = threads.find(id);
            if (it != threads.end())
                return it->second;
            else
                return NULL;
        }

        bool init(int threadNum);

        void release();

        void finalAll();

        void joinAll();
    
};

}

#endif
