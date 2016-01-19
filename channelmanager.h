#ifndef __CHANNELMANAGER_H
#define __CHANNELMANAGER_H

#include <map>

namespace fluid 
{

    class ChannelManager {
        private:
            std::map<unsigned int, Channel*> channels;
        public:
            Channel* get(unsigned int id) {
                std::map<unsigned int, Channel*>::iterator it = channels.find(id);
                if (it == channels.end()) {
                    return NULL;
                } else {
                    return it->second;
                }
            }

            void put(Channel* channel) {
                if (channel == NULL)
                    return;
                channels.insert(std::make_pair(channel->id(), channel));
            }

            void remove(unsigned int id) {
                channels.erase(id);
            }
    };

}

#endif
