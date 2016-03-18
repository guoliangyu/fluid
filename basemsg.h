#ifndef __BASE_MSG_H
#define __BASE_MSG_H

#include <string.h>

namespace fluid
{

class BaseMsg:public RawMessage {
    private:
        ENetPacket *_packet;
    public:
        BaseMsg(ENetPacket *p):_packet(p) {
            
        }
        
        BaseMsg(unsigned char* data, int len) {
            _packet = enet_packet_create(data, len, ENET_PACKET_FLAG_RELIABLE);
        }

        ~BaseMsg() {
            if (_packet != NULL)
                enet_packet_destroy (_packet);
        }

        unsigned char* getData(){
            return _packet->data;
        }

        int size() {
            return _packet->dataLength;
        }
        
        void swap(BaseMsg& message) {
            std::swap(message._packet, this->_packet);
        }
        
        ENetPacket* packet(){
            return _packet;
        }

};

}

#endif
