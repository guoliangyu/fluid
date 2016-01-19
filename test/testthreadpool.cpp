#include "handlerthreadpool.h"
#include "singleton.h"
#include "basemsg.h"
#include <stdlib.h>
#include <stdio.h>

class EchoHandler: public fluid::Handler, public fluid::Singleton<EchoHandler>
{
    public:
        bool process(fluid::Channel* channel, fluid::Message* message){
            fluid::BaseMsg* msg = (fluid::BaseMsg*)message;
            char *str = (char*)malloc(msg->size()+1);
            memcpy(str, msg->getData(), msg->size());
            str[msg->size()] = 0;
            printf("%s\n", str);
        }
        
};

class EchoThreadFactory : public fluid::HandlerThreadFactory, public fluid::Singleton<EchoThreadFactory>
{
    public:
        fluid::HandlerThread* makeHandlerThread(){
            return new fluid::HandlerThread(EchoHandler::instance());
        }
};

int main(int argc, const char* argv[])
{
    int threadNum = 3;
    int msgNum = 10000;
    if (argc == 3){
        threadNum = atoi(argv[1]);
        msgNum = atoi(argv[2]);
    }
    fluid::HandlerThreadPool *pool = new fluid::HandlerThreadPool(EchoThreadFactory::getMe());
    pool->init(threadNum);
    char data[100];
    for(int i = 0; i < msgNum; i++){
        snprintf(data, sizeof(data), "echo test %d", i);
        fluid::BaseMsg *msg = new fluid::BaseMsg((unsigned char*)data, strlen(data));
        pool->post(NULL, msg);
        ::usleep(1);
    }
    ::usleep(100000000);
    pool->finalAll();
    pool->joinAll();
    return 0;
}

