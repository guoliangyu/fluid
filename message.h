#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <memory.h>
#include <stdlib.h>
#include <algorithm>

namespace fluid
{

enum MsgType {
    MSG_TYPE_EVENT,
    MSG_TYPE_RAW,
    MSG_TYPE_USER
};

class Message
{
    private:
        MsgType msgType;
    public:
        Message(MsgType t):msgType(t){
        
        }

        virtual ~Message(){};

        MsgType type() {
            return msgType;
        }
        
        /**
         * 序列化长度
         * @return 
         */
        virtual int byteSize() const = 0;
        
        /**
         * 序列化函数
         * @param buf 目标缓冲区
         * @param buflen 目标缓冲区大小
         * @return <0 表示序列化出错, >=0为有效数据长度, 0表示不支持序列化,结果无效
         */
        virtual int serialize(unsigned char* buf, int buflen) const = 0;
        
};

class RawMessage : public Message
{
public:
    RawMessage():Message(MSG_TYPE_RAW) {
        
    }
    
    virtual unsigned char* getData() = 0;
    virtual int size() = 0;
    
private:
    int byteSize() const {
        return 0;
    }
    
    int serialize(unsigned char* buf, int buflen) const {
        return 0;
    }
    
};

class BareMessage : public RawMessage
{
private:
    unsigned char *_data;
    int _len;
public:
    BareMessage(const unsigned char* data, int len) {
        if (data != NULL && len > 0) {
            _data = (unsigned char*)malloc(len);
            memcpy(_data, data, len);
            _len = len;
        }
    }
    
    unsigned char* getData() {
        return _data;
    }
    
    int size() {
        return _len;
    }
    
    void swap(BareMessage& message) {
        std::swap(message._data, this->_data);
        std::swap(message._len, this->_len);
    }
    
};

}

#endif
