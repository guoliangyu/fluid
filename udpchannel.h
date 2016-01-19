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
            HandlerThreadPool* firstHandler;
        public:
            UdpChannel(ENetPeer* p, HandlerThreadPool* handler):peer(p),firstHandler(handler) {

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

            void send(const unsigned char* data, int len);

            void flush();

            void input(const unsigned char* data, int len);

            void onCreate();

            void onClose();

    };

}
