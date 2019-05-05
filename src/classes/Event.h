#ifndef __EVENT__
#define __EVENT__

#include <string>

#define EDIT_INPUT_MESSAGE  0
#define SEND_INPUT_MESSAGE  1
#define RESTORE_MESSAGES    2

struct Message_data
{
    int id;
    int time;
    int from;
    int to;
    const std::wstring text;
};

union Event_data
{
    std::wstring* message_pointer;
    int messages_count;
    Message_data* message_data;
};

struct Event
{
    int type;
    union Event_data data;

    Event(int _type, Event_data _data);
};



#endif