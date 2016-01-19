#ifndef __BASE_MSG_H
#define __BASE_MSG_H

#include <string.h>

namespace fluid
{

class BaseMsg:public Message {
    private:
        char* data;
        int len;
    public:
        BaseMsg(const unsigned char* d, int dlen) {
            data = (char*)malloc(dlen);
            if (data != NULL) {
                memcpy(data, d, dlen);
                len = dlen;
            }
        }

        ~BaseMsg() {
            if (data != NULL)
                free(data);
        }

        char* getData(){
            return data;
        }

        int size() {
            return len;
        }

};

}

#endif
