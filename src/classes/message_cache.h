#ifndef __MESSAGE_CACHE__
#define __MESSAGE_CACHE__

#include <vector>
#include "messages.h"

class Message_cache
{
    std::vector<Message*> cache;
public:
    
    bool is_empty();
    void push(Message* _msg);
    Message* last();

    Message* operator[](int n);
    
};


#endif