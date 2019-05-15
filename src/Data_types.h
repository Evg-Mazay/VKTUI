#ifndef __EVENT__
#define __EVENT__

#include <string>

#define EDIT_INPUT_MESSAGE  0
#define SEND_INPUT_MESSAGE  1
#define RECIEVED_MESSAGE    2
#define RESTORE_MESSAGES    3
#define EXIT                4
#define KEY_PRESS           5

/*
    Here is data types to use in program
*/

struct credentials
{
    int id;
    std::string token;
};

struct dialog
{
    int id;
    std::wstring name;
};

struct Message_data
{
    int id;
    long time;
    int person; // it means "from who" for incoming msg, and "to who" for outcoming
    std::wstring text;
};

union Event_data
{
    int integer;
    Message_data* message_p;    // need to be freed
    std::wstring* text_p;       // need to be freed
    const std::wstring* no_free_text_p;
};

struct Event
{
    int type;
    Event_data data;

    Event(int _type){type = _type;}
    Event(int _type, Event_data _data) { type = _type; data = _data;}

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
    Event(int _type, std::wstring* _data) {type = _type; data.no_free_text_p = _data;}

};



#endif
