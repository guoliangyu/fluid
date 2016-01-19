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
        enet_host_flush (peer -> host); 
    }

    void UdpChannel::input(const unsigned char* data, int len) {
        if (firstHandler) {
            BaseMsg *msg = new BaseMsg(data, len);
            if (msg && NULL != firstHandler) {
                firstHandler->post(this, msg);
            }
        }
        
    }

    void UdpChannel::onCreate() {
        ChannelEvent *e = new ChannelEvent(CHANNEL_EVENT_OPEN);
        firstHandler->post(this, e);
    }

    void UdpChannel::onClose() {
        ChannelEvent *e = new ChannelEvent(CHANNEL_EVENT_CLOSE);
        firstHandler->post(this, e);
    }

}
