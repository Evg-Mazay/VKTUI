#ifndef __EVENT_HANDLER__
#define __EVENT_HANDLER__

#include <mutex>

#include "state.h"
#include "event_queue.h"
#include "classes/events.h"

class Event_handler
{
    bool* killswitch;

    mutex event_handler_mutex;
    State* state;
    Event_queue* event_queue;

public:
    Event_handler(State* _state, Event_queue* _event_queue, bool* _killswitch);
    
    void main_loop();
};

#endif