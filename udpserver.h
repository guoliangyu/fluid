#ifndef __UDP_SERVER_H
#define __UDP_SERVER_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include "handlerthreadpool.h"
#include <enet/enet.h>

namespace fluid 
{

    class UdpServer {
        private:
            ENetHost *server;
            HandlerThreadPool* firstHandler;

            const static int DEFAULT_PEER_LIMIT = 4000;
            const static int DEFAULT_CHANNEL_LIMIT = 1;
        public:

            UdpServer():server(NULL),firstHandler(NULL){
            }

            bool init(int port, int maxPeer, int channelNum); 
            bool init(int port); 
            void run(); 
            void release();

            void addLastHandlerPool(HandlerThreadPool *threadPool);

    };

}

#endif

