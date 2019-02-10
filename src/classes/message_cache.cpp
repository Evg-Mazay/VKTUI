#include "message_cache.h"
#include <stdexcept>


void Message_cache::push(Message* _msg)
{
    cache.push_back(_msg);
}

Message* Message_cache::last()
{
    return cache.back();
}

bool Message_cache::is_empty()
{
    return cache.empty();
}

Message* Message_cache::operator[](int n)
{
    try
    {
        cache.at(n);
    }
    catch (std::out_of_range)
    {
        return NULL;
    }

    return cache[n];
}