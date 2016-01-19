#ifndef _BYTE_BUFFER_H
#define _BYTE_BUFFER_H

#include <vector>

class ByteBuffer
{
    private:
        int maxSize;
        int offPtr;
        int curPtr;
        std::vector<unsigned char> buffer;
    public:

        const static int TRUCK_SIZE = 64 * 1024;

        ByteBuffer():maxSize(TRUCK_SIZE),offPtr(0),curPtr(0),buffer(maxSize){

        }

        void put(const unsigned char* buf, const int size) {
            wrReserve(size);
            bcopy(buf, &(buffer[curPtr]), size);
            curPtr += size;
        }

        unsigned char* wrBuf() {
            return &(buffer[curPtr]);
        }

        unsigned char* rdBuf() {
            return &(buffer[offPtr]);
        }

        bool rdReady() const {
            return curPtr > offPtr;
        }

        int rdSize() const {
            return curPtr - offPtr;
        }

        void rdFlip(int size) {
            offPtr += size;
            if (curPtr > offPtr) {
                int tmp = curPtr - offPtr;
                if (offPtr >= tmp){
                    memmove(&(buffer[0]), &(buffer[offPtr]), tmp);
                    offPtr = 0;
                    curPtr = tmp;
                }
            } else {
                offPtr = 0;
                curPtr = 0;
            }
        }

        int wrSize() const {
            return maxSize - curPtr;
        }

        void wrFlip(const int size) {
            curPtr += size;
        }

        void reset() {
            offPtr = 0;
            curPtr = 0;
        }

        int getMaxSize() {
            return maxSize;
        }

        int getOffPtr() {
            return offPtr;
        }

        int getCurPtr() {
            return curPtr;
        }

        void wrReserve(const int size) {
            if (wrSize() < size) {
                maxSize += maxSize;
                buffer.resize(maxSize); 
            }
        }
};

#endif
