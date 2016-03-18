#ifndef __IDPOOL_H
#define __IDPOOL_H

#include "mutex.h"
#include <set>
#include <list>
#include "idpool.h"

namespace fluid 
{
    template<class T>
    class IdPool
    {
        private:
            std::set<T> idSet;
            std::list<T> idList;
            typedef typename std::set<T>::iterator IdSetIter;
            typedef typename std::list<T>::iterator IdListIter;
            T maxId;
            T minId;
            T curId;
            Mutex *lock;
        protected:
            void initRange(T min, T max) {
               maxId = max;
               minId = min;
               curId = min;
            }
        public:
            IdPool(bool mutex = false)
            {
                if (mutex)
                    lock = new Mutex();
                else
                    lock = NULL;
                initRange(1,(T)-1); 
            }

            IdPool(T start, bool mutex = false)
            {
                if (mutex)
                    lock = new Mutex();
                else
                    lock = NULL;
                initRange(start, (T)-1);
            }

            IdPool(T start, T end, bool mutex = false)
            {
                if (mutex)
                    lock = new Mutex();
                else
                    lock = NULL;
                initRange(start, end);
            }

            ~IdPool() {
                if (lock != NULL)
                    delete lock;
            }
            
            T get(T &id){
                if (id != 0) {
                    return 0;
                }
                T ret;
                if (lock != NULL)
                    lock->lock();
                if (maxId > curId){
                    ret = curId;
                    curId++;
                } else {
                    ret = maxId;
                }
                if (ret == maxId && !idList.empty()) {
                    ret = idList.back();
                    idList.pop_back();
                    idSet.erase(ret);
                }
                if (lock != NULL)
                    lock->unlock();
                id = ret;
                return ret;
            }

            T get() {
                T ret;
                if (lock != NULL)
                    lock->lock();
                if (maxId > curId){
                    ret = curId;
                    curId++;
                } else {
                    ret = maxId;
                }
                if (ret == maxId && !idList.empty()) {
                    ret = idList.back();
                    idList.pop_back();
                    idSet.erase(ret);
                }
                if (lock != NULL)
                    lock->unlock();
                return ret;
            }

            void put(T id) {
                if (lock != NULL)
                    lock->lock();
                if (id < maxId && id >= minId) {
                    if (idSet.insert(id).second)
                        idList.push_front(id);
                }
                if (lock != NULL)
                    lock->unlock();
            }

            bool invalid(T id) {
                return id >= minId && id < maxId;
            }

    };

}

#endif

