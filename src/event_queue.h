#ifndef __EVENT_QUEUE__
#define __EVENT_QUEUE__

#include <mutex>
#include <condition_variable>
#include <vector>

#include "classes/events.h"

using namespace std;

class Event_queue
{
    mutex queue_mutex;
    condition_variable new_events;

    std::vector<Event*> queue;
public:
    void push(Event* _event);
    Event* pop();

    bool is_empty();
    void notify_event_handler();
    void event_handler_wait(unique_lock<mutex> * lck);
    
};

#endif