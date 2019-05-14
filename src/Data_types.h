#ifndef __EVENT__
#define __EVENT__

#include <string>

#define EDIT_INPUT_MESSAGE  0
#define SEND_INPUT_MESSAGE  1
#define RESTORE_MESSAGES    2
#define EXIT                3
#define KEY_PRESS           4

/*
    Here is data types to use in program
*/


struct dialog
{
    int id;
    std::wstring name;
};

struct Message_data
{
    int id;
    long time;
    int person;
    std::wstring text;
};

union Event_data
{
    int integer;
    Message_data* message_p;
    std::wstring* text_p;
};

struct Event
{
    int type;
    Event_data data;

    Event(int _type){type = _type;}
    Event(int _type, Event_data _data) { type = _type; data = _data;}
    Event(int _type, Message_data* _data) {type = _type, data.message_p = _data;}

    Event(int _type, Message_data _data)
    {
        type = _type;
        Message_data* allocated_data = new Message_data();
        *allocated_data = _data;
        data.message_p = allocated_data;
    }

    Event(int _type, std::wstring _data)
    {
        type = _type;
        std::wstring* allocated_data = new std::wstring();
        *allocated_data = _data;
        data.text_p = allocated_data;
    }

    Event(int _type, int _data) { type = _type; data.integer = _data;}
    Event(int _type, std::wstring* _data) {type = _type; data.text_p = _data;}

};



#endif
