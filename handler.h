#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "channel.h"
#include "message.h"
#include "channelevent.h"

namespace fluid
{

class Handler
{
    public:
        Message* postMsg(Channel* channel, Message* msg) {
            if(msg == NULL)
                return NULL;
            if (msg->type() == MSG_TYPE_USER || msg->type() == MSG_TYPE_RAW)
                return this->process(channel, msg);
            else if (msg->type() == MSG_TYPE_EVENT) {
                ChannelEvent *event = (ChannelEvent*)msg;
                if(event->isEvent(CHANNEL_EVENT_OPEN))
                    onCreate(channel, event);
                else if (event->isEvent(CHANNEL_EVENT_CLOSE)) 
                    onClose(channel, event);
            }
            return msg;
        }

        virtual Message* process(Channel* channel, Message* msg) = 0;

        virtual void onCreate(Channel* channel, ChannelEvent* event) = 0;
        virtual void onClose(Channel* channel, ChannelEvent* event) = 0;

};

}

#endif
