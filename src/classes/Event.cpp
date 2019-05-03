// #include <


#include "Event.h"



Event::Event(int _type, Event_data _data)
{
    type = _type;
    data = _data;
}

int Event::get_type()
{
    return type;
}

union Event_data Event::get_data()
{
    return data;
}



Event_data Event_factory::get_event_data_message(const std::wstring* text)
{
    Event_data ev;
    Message_data* msg = new Message_data();
    msg->text = text;
    ev.message_data = *msg;
    
    return ev;
}

Event_data Event_factory::get_event_data_message(std::wstring text)
{
    Event_data ev;
    Message_data* msg = new Message_data();
    msg->text = new std::wstring(text);
    ev.message_data = *msg;
    
    return ev;
}
