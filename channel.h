#ifndef __CHANNEL_H
#define __CHANNEL_H

#include "message.h"


namespace fluid {

    class HandlerThreadPool;

    class Channel {
    protected:
        void* userData;
        HandlerThreadPool* firstHandler;
        HandlerThreadPool* lastHandler;
    public:

        Channel(HandlerThreadPool* pool);

        void setUserData(void *d) {
            userData = d;
        }

        void* getUserData() {
            return userData;
        }

        virtual unsigned int id() = 0;

        void input(Message* msg);

        void output(Message* msg);

        virtual void send(const unsigned char* data, int len) = 0;

        virtual void flush() = 0;

        void onCreate();

        void onClose();

        void sendAndFlush(const unsigned char* data, int len) {
            send(data, len);
            flush();
        }
    };

}

#endif
