#ifndef __THREAD_H__
#define __THREAD_H__

#include "sys.h"
#include "mutex.h"
#include "cond.h"
#include <string>
#include "idpool.h"

namespace fluid
{

class Thread : private Noncopyable
{
    private:
        std::string name;
        volatile bool alive;
        volatile bool complete;
        pthread_t thread;
        bool joinable;
        int id;

        static IdPool<int> idPool;
    public:
        Thread(const std::string& n = std::string("thread"), bool joinable = true)
            :name(n), alive(false), complete(false), thread(0), joinable(joinable)
        {
            id = idPool.get();
        }
        virtual ~Thread(){};


        int getId() {
            return id;
        }

        static pthread_t getThreadId()
        {
            return ::pthread_self();
        }

        static void msleep(const long msec)
        {
            ::usleep(1000 * msec);
        }

        const bool isJoinable()const
        {
            return joinable;
        }

        const bool isAlive() const
        {
            return alive;
        }

        bool start();
        void join();

        void final() {
            complete = true;
        }

        const bool isFinal() const
        {
            return complete;
        }

        virtual void run() = 0;

        const std::string& getName() const {
            return name;
        }


        static void* threadFunc(void*);
};

}

#endif
