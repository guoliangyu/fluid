#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <map>
#include <vector>
#include "thread.h"
#include "threadfactory.h"

namespace fluid
{

class ThreadPool
{
    protected:
        std::vector<Thread*> threads;
        ThreadFactory& factory;

    public:
        ThreadPool(ThreadFactory& f):factory(f)
        {
        }

        bool addThread(Thread* thread) {
            if(thread == NULL)
                return false;
            threads.push_back(thread);
            return true;
        }

        Thread* getThreadByIndex(int idx) {
            if (idx >= threads.size())
                return NULL;
            return threads[idx];
        }

        bool init(int threadNum);

        void release();

        void finalAll();

        void joinAll();
    
};

}

#endif
