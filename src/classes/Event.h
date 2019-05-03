#ifndef __EVENT__
#define __EVENT__

#include <string>

#define EDIT_INPUT_MESSAGE 0
#define SEND_INPUT_MESSAGE 1

struct Message_data
{
    const std::wstring* text;
};

union Event_data
{
    Message_data message_data;
};

class Event
{
    int type;
    union Event_data data;

public:
    Event(int _type, Event_data _data);
    int get_type();
    union Event_data get_data();
};

class Event_factory
{
public:
    Event_data get_event_data_message(const std::wstring* text);
    Event_data get_event_data_message(std::wstring text);

    
};



#endif