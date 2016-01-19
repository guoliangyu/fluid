#ifndef __MESSAGE_H__
#define __MESSAGE_H__

namespace fluid
{

enum MsgType {
    MSG_TYPE_EVENT,
    MSG_TYPE_USER
};

class Message
{
    private:
        MsgType msgType;
    public:
        Message(MsgType t=MSG_TYPE_USER):msgType(t){
        
        }

        virtual ~Message(){};

        MsgType type() {
            return msgType;
        }
};

}

#endif
