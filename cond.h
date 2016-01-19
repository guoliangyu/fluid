#ifndef __COND_H__
#define __COND_H__

#include "sys.h"
#include "noncopyable.h"
#include "mutex.h"

namespace fluid
{

class Cond : private Noncopyable
{
    private:
        pthread_cond_t cond;
    public:
        Cond();
        ~Cond();
        void broadcast();
        void signal();
        void wait(Mutex& mutex);
};

}

#endif
