#include "../include/enet/enet.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    ENetHost *client = enet_host_create(NULL, 1, 1, 256 * 1024, 128 * 1024);
    if (client == NULL){
        printf("create client error\n");
        return 1;
    }
    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 10001;
    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == NULL) {
        printf("fail to connect.\n");
        return 1;
    }
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        printf("connect success.\n");
    } else {
        enet_peer_reset(peer);
        printf("connect fail.\n");
        return 1;
    }
    int i = 0;
    char tmp[256];
    while(1) {
        enet_host_service(client, &event, 1000);
        sprintf(tmp, "%d", ++i);
        ENetPacket *packet = enet_packet_create(tmp, strlen(tmp), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
        enet_host_flush(client);
    }
    
    return 0;
}

