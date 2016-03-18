#include "udpchannel.h"
#include "basemsg.h"
#include "channelevent.h"

namespace fluid 
{

    void UdpChannel::send(const unsigned char* data, int len) {
        ENetPacket *packet = enet_packet_create(data, len, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

    void UdpChannel::flush() {
        
    }

}
