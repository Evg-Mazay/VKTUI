#include "events.h"
#include "messages.h"

#include "../event_handler.h"


Event_new_message::Event_new_message(Message* _msg)
{
    msg = _msg;
}

Event_new_message::~Event_new_message()
{

}

void Event_new_message::process(State* state)
{
    state->push_debug_message(msg->get_text());
}
