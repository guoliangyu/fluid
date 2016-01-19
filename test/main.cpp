#include "../udpserver.h"
#include "../singleton.h"
#include "../basemsg.h"
#include "../channel.h"
#include "../channelevent.h"

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

        void onCreate(fluid::Channel* channel, fluid::ChannelEvent* event) {
            printf("channel create.\n");
        }

        void onClose(fluid::Channel* channel, fluid::ChannelEvent* event) {
            printf("channel close.\n");
        }

};

class EchoThreadFactory : public fluid::HandlerThreadFactory, public fluid::Singleton<EchoThreadFactory>
{
    public:
        fluid::HandlerThread* makeHandlerThread(){
            return new fluid::HandlerThread(EchoHandler::getMe());
        }
};

int main(int argc, const char* argv[])
{
    fluid::UdpServer* server = new fluid::UdpServer();
    fluid::HandlerThreadPool *pool = new fluid::HandlerThreadPool(EchoThreadFactory::getMe());
    pool->init(10);
    server->init(10001);
    server->addLastHandlerPool(pool);
    server->run();
    server->release();
}

