#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace fluid
{

class Noncopyable
{
    protected:
        Noncopyable() {};
        ~Noncopyable() {};
        
    private:
        Noncopyable(const Noncopyable&);
        const Noncopyable & operator= (const Noncopyable &);
};

}
#endif

