#include <unistd.h>

#include "B.h"
#include "event_queue.h"
#include "classes/events.h"
#include "classes/messages.h"

using namespace std;


B::B(Event_queue* _event_queue, bool* _killswitch)
{
    event_queue = _event_queue;
    killswitch = _killswitch;
}

void B::debug_generate_event()
{
    sleep(2);

    int id = time(0)%1000;
    Message* msg = new Message(id, "test event msg, id: " + to_string(id));

    Event* event = new Event_new_message(msg);

    event_queue->push(event);
}