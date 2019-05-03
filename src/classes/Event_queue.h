#ifndef __QUEUE__
#define __QUEUE__

#include <mutex>
#include <vector>

#include "Event.h"

class Event_queue
{
    std::vector<Event> data;
    std::mutex queue_mutex;

public:
    int empty();
    void push(Event);
    Event pop();
    
};



#endif