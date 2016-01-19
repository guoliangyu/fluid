#include <queue>
#include <stdio.h>

#define QUEUE_SIZE 10240

template<typename T>
class MsgQueue{
    protected:
        typedef std::pair<volatile bool, T*> ElemType;
        std::queue<T*, std::deque<T*> > _buff;
        ElemType _queue[QUEUE_SIZE];
        int _read;
        int _write;
        void flush(){
            while(!_buff.empty() && !_queue[_write].first){
                _queue[_write].second = _buff.front();
                __asm__ __volatile ("sfence":::);
                _queue[_write].first = true;
                _write = (_write + 1) % QUEUE_SIZE;
                _buff.pop();
            }
        }
    public:
        MsgQueue() {
            _read = 0;
            _write = 0;
            for(int i = 0; i < QUEUE_SIZE; i++){
                _queue[i].first = false;
            }
        }

        T* get(){
            if(_queue[_read].first)
                return _queue[_read].second;
            else
                return NULL;
        }

        void push(T* t){
            flush();
            if(_queue[_write].first)
                _buff.push(t);
            else {
                _queue[_write].second = t;
                __asm__ __volatile ("sfence":::);
                _queue[_write].first = true;
                _write = (_write + 1) % QUEUE_SIZE;
            }
        }

        void pop() {
            _queue[_read].first = false;
            _read = (_read + 1) % QUEUE_SIZE;
        }

};

