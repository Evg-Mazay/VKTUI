#include <mutex>

#include "Event_queue.h"
#include "Event.h"

int Event_queue::empty()
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    return data.empty();
}

void Event_queue::push(Event event)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    data.push_back(event);
}

Event Event_queue::pop()
{
    std::lock_guard<std::mutex> lock(queue_mutex);

    Event event = data.back();
    data.pop_back();
    return event;
}
