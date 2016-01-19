#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

class Scheduler
{
    public:
        virtual int schedule(Channel* channel, void* msg) = 0;
};

#endif
