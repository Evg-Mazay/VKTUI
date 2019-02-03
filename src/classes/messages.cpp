

#include "messages.h"

Message::Message(int _id, string _text)
{
    id = _id;
    text = _text;
}

int Message::get_id()
{
    return id;
}

string Message::get_text()
{
    return text;
}
