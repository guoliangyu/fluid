#include "idpool.h"
#include "bytebuffer.h"
#include "handlerthreadpool.h"
#include "sys.h"
#include "enet/enet.h"
#include "basemsg.h"

namespace fluid 
{

    class UdpChannel : public Channel
    {
        private:
            ENetPeer* peer;
        public:
            UdpChannel(ENetPeer* p, HandlerThreadPool* handler):peer(p),Channel(handler) {
                
            }

            ~UdpChannel() {
                enet_peer_reset(peer);
            }

            unsigned int id(){
                if (peer)
                    return peer->incomingPeerID;
                else
                    return 0;
            }

            virtual void send(const unsigned char* data, int len);

            virtual void flush();

    };

}
