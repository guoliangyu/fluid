#include "cond.h"

namespace fluid
{

Cond::Cond()
{
    ::pthread_cond_init(&cond,NULL);
}

Cond::~Cond()
{
    ::pthread_cond_destroy(&cond);
}

void Cond::broadcast()
{
    ::pthread_cond_broadcast(&cond);
}

void Cond::signal()
{
    ::pthread_cond_signal(&cond);
}

void Cond::wait(Mutex& mutex)
{
    ::pthread_cond_wait(&cond,&(mutex.mutex));
}

}

