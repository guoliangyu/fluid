#include<stdio.h>
#include "enet/enet.h"
#include "udpserver.h"
#include "udpchannel.h"

namespace fluid 
{

    bool UdpServer::init(int port, int maxPeer, int channelNum) {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;
        this->server = enet_host_create (& address, 
                maxPeer,
                channelNum,
                0,
                0);
        if (this->server == NULL) {
            fprintf (stderr, "An error occurred while trying to create an ENet server host.\n");
            return false;
        }

        RecycleThreadPool *pool = new RecycleThreadPool();
        pool->init(1);
        this->firstHandler = pool; 
	
    }

    bool UdpServer::init(int port) {
        return init(port, DEFAULT_PEER_LIMIT, DEFAULT_CHANNEL_LIMIT);
    }

    void UdpServer::release() {
        if (this->server != NULL) {
            enet_host_destroy(this->server);
            this->server = NULL;
        }
        HandlerThreadPool *pool = firstHandler;
        while(pool){
            HandlerThreadPool *next = pool->getNextPool();
            pool->release();
            pool = next;
        }
    }

    void UdpServer::run() {
        ENetEvent event;
        /* Wait up to 1000 milliseconds for an event. */
        int pollret = 0;
        while (1)
        {
            pollret = enet_host_service (server, & event, 1000);
            if (pollret > 0) {
                switch (event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                        {
                            if (NULL != event.peer) {
                                UdpChannel *newChannel = new UdpChannel(event.peer, firstHandler);
                                event.peer -> data = newChannel;
                                newChannel->onCreate();
                            }
                        }
                        break;
                    case ENET_EVENT_TYPE_RECEIVE:
                        {
                            UdpChannel *channel = (UdpChannel*)(event.peer->data);
                            if (channel != NULL) 
                                channel->input(event.packet->data, event.packet->dataLength);
                            enet_packet_destroy (event.packet);
                        }
                        break;
                    case ENET_EVENT_TYPE_DISCONNECT:
                        {
                            if (event.peer -> data != NULL) {
                                UdpChannel *channel = (UdpChannel*)(event.peer->data);
                                if (channel != NULL)
                                    channel->onClose();
                            }
                        }
                }
            } else if (pollret < 0) {
                fprintf(stderr, "udpserver error!!!");
                break;
            }
        }
    }

    void UdpServer::addLastHandlerPool(HandlerThreadPool *threadPool){
        if(firstHandler){
            if (firstHandler->getNextPool() == NULL){
                threadPool->setNextPool(firstHandler);
                firstHandler = threadPool;
            } else {
                HandlerThreadPool* last = firstHandler;
                HandlerThreadPool* cur = firstHandler->getNextPool();
                while(cur->getNextPool() != NULL){
                    last = cur;
                    cur = cur->getNextPool();
                }
                last->setNextPool(threadPool);
                threadPool->setNextPool(cur);
            }
        }
        else
            firstHandler = threadPool;
    }

}

