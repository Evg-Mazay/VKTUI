#ifndef __EVENTS__
#define __EVENTS__

#include <vector>
#include <string>

#include "messages.h"

class State;

class Event
{
public:
    virtual ~Event(){}
    virtual void process(State* state) = 0;
};


class Event_new_message: public Event
{
    Message* msg;
public:
    Event_new_message(Message* _msg);
    ~Event_new_message();

    void process(State* state);
};


#endif