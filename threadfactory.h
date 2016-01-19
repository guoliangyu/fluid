#ifndef __THREAD_FACTORY_H__
#define __THREAD_FACTORY_H__

namespace fluid
{

class ThreadFactory
{
    public:
        virtual Thread* makeThread() = 0;
};

}

#endif
