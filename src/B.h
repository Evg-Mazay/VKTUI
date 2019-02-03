#ifndef __B__
#define __B__

#include "event_queue.h"

class B
{
    bool* killswitch;
    Event_queue* event_queue;
public:
    B(Event_queue* _event_queue, bool* _killswitch);

    void debug_generate_event();
    
};

#endif