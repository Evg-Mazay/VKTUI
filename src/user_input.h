#ifndef __USER_INPUT__
#define __USER_INPUT__

#include "state.h"
#include "event_queue.h"

class User_input
{
    bool* killswitch;

    State* state;
    Event_queue* event_queue;
public:
    User_input(State* _state, Event_queue* _event_queue, bool* _killswitch);

    void main_loop();
    
};

#endif