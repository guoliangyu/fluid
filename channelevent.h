#ifndef __CHANNEL_EVENT_H__
#define __CHANNEL_EVENT_H__

#include "message.h"

namespace fluid
{

    enum ChannelEventType {
        CHANNEL_EVENT_OPEN = 0,
        CHANNEL_EVENT_CLOSE = 1
    };

    class ChannelEvent : public Message
    {
        private:
            ChannelEventType eType;
        public:
            ChannelEvent(ChannelEventType t):eType(t),Message(MSG_TYPE_EVENT){
            }

            bool isEvent(ChannelEventType t){
                return this->eType == t;
            }

    };

}

#endif
