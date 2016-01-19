#include "thread.h"

namespace fluid 
{
IdPool<int> Thread::idPool(10240, true);

void *Thread::threadFunc(void *arg)
{
    Thread *thread = (Thread*)arg;
    thread->alive = true;
    sigset_t sig_mask;
    sigfillset(&sig_mask);
    pthread_sigmask(SIG_SETMASK, &sig_mask, NULL);
    thread->run();
    thread->alive=false;
    if (!thread->isJoinable())
    {
        delete thread;
    }
    pthread_exit(thread);
}

bool Thread::start()
{
    if (alive)
        return true;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (!joinable)
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(0!=pthread_create(&thread,NULL,Thread::threadFunc,this))
        return false;
    return true;
}

void Thread::join()
{
    if (0!=thread && joinable )
    {
        ::pthread_join(thread, NULL);
        thread = 0;
    }
}

}

