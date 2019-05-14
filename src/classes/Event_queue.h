#ifndef __QUEUE__
#define __QUEUE__

#include <mutex>
#include <vector>

#include "../Data_types.h"

/*
This is the queue with mutex - a part of backend.

Backend.queue <-- Network: Events
Backend.queue <-- User_input: Events

*/


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