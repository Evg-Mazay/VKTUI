

#include "messages.h"

Message::Message(int _id, wstring _text)
{
    id = _id;
    text = _text;
}

int Message::get_id()
{
    return id;
}

wstring Message::get_text()
{
    return text;
}
