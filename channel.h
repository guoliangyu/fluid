#ifndef __CHANNEL_H
#define __CHANNEL_H

namespace fluid {

class Channel {
    private:
        void* userData; 
    public:
        Channel():userData(NULL){
        }

        void setUserData(void *d) {
            userData = 0;
        }
        void* getUserData() {
            return userData;
        }
        virtual unsigned int id() = 0;
        virtual void send(const unsigned char* data, int len) = 0;
        virtual void flush() = 0;
        virtual void onCreate() = 0;
        virtual void onClose() = 0;
        void sendAndFlush(const unsigned char* data, int len) {
            send(data,len);
            flush();
        }
};

}

#endif
