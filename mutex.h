#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "sys.h"
#include "noncopyable.h"

namespace fluid 
{

class Mutex : private Noncopyable
{
    private:

        pthread_mutex_t mutex;

    public:
        friend class Cond;

        enum MutexType
        {
            Fast = PTHREAD_MUTEX_FAST_NP,
            Recursive = PTHREAD_MUTEX_RECURSIVE_NP,
            ErrorCheck = PTHREAD_MUTEX_ERRORCHECK_NP
        };

        Mutex(MutexType t = Fast) {
            pthread_mutexattr_t attr;
            ::pthread_mutexattr_init(&attr);
            ::pthread_mutexattr_settype(&attr, t);
            ::pthread_mutex_init(&mutex, &attr);
        }

        ~Mutex() { ::pthread_mutex_destroy(&mutex);}

        void lock() { ::pthread_mutex_lock(&mutex); }

        void unlock() { ::pthread_mutex_unlock(&mutex); }

        bool trylock() { return (::pthread_mutex_trylock(&mutex) == 0); }
};

class MutexScopeLock : private Noncopyable
{
    private:
        Mutex &mlock;
        bool locked;
    public:
        MutexScopeLock(Mutex& m) : mlock(m), locked(false) {
            locked = mlock.trylock();
        }

        ~MutexScopeLock() {
            if (locked)
                mlock.unlock();
        }
};

}
#endif
