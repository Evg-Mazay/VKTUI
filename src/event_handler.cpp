#include <unistd.h>


#include "event_handler.h"

Event_handler::Event_handler(State* _state, Event_queue* _event_queue, bool* _killswitch)
{
    state = _state;
    killswitch = _killswitch;
    event_queue = _event_queue;
}

void Event_handler::main_loop()
{
    unique_lock<mutex> lck(event_handler_mutex);
    event_queue->event_handler_wait(&lck);

    while (! event_queue->is_empty())
    {
        Event* event = event_queue->pop();

        event->process(state);

        delete event;

        state->notify_renderer();
    }
}