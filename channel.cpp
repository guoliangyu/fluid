/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   channel.cpp
 * Author: netease
 *
 * Created on 2016年3月8日, 下午5:11
 */

#include "channel.h"
#include "handlerthreadpool.h"

namespace fluid {

    Channel::Channel(HandlerThreadPool* pool) : userData(NULL),firstHandler(pool), lastHandler(NULL){
        if (firstHandler) {
            lastHandler = firstHandler;
            while (lastHandler->getNextPool())
                lastHandler = lastHandler->getNextPool();
        }
    }

    void Channel::input(Message* msg) {
        if (firstHandler) {
            firstHandler->forwardMessage(this, msg);
        } else {
            delete msg;
        }
    }

    void Channel::output(Message* msg) {
        if (lastHandler) {
            lastHandler->backwardMessage(this, msg);
        } else {
            delete msg;
        }
    }

    void Channel::onCreate() {
        if (firstHandler) {
            ChannelEvent *e = new ChannelEvent(CHANNEL_EVENT_OPEN);
            firstHandler->forwardMessage(this, e);
        }
    }

    void Channel::onClose() {
        if (firstHandler) {
            ChannelEvent *e = new ChannelEvent(CHANNEL_EVENT_CLOSE);
            firstHandler->forwardMessage(this, e);
        }
    }
    
    
}


