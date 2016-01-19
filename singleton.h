#ifndef _SINGLETON_H
#define _SINGLETON_H

namespace fluid
{

template <typename T>
class Singleton
{
    private:
        Singleton(const Singleton&);
        const Singleton & operator=(const Singleton &);
    protected:
        static T* ms_Singleton;
        Singleton(void)
        {
        }
        ~Singleton(void)
        {
        }
    public :
        static void delMe(void)
        {
            if(ms_Singleton)
            {
                delete ms_Singleton;
                ms_Singleton=0;
            }
        }
        static T* instance(void)
        {
            if(!ms_Singleton)
            {
                ms_Singleton = new T();
            }
            return ms_Singleton;
        }
        static T& getMe(void)
        {
            return *instance();
        }
};
template <typename T> T* Singleton<T>::ms_Singleton=0;

}

#endif
