

#include "event_queue.h"
#include "classes/events.h"

using namespace std;

bool Event_queue::is_empty()
{
    lock_guard<mutex> lck(queue_mutex);
    return queue.empty();
}

void Event_queue::push(Event* _event)
{
    lock_guard<mutex> lck(queue_mutex);

    queue.push_back(_event);
    
    notify_event_handler();
}

Event* Event_queue::pop()
{
    lock_guard<mutex> lck(queue_mutex);

    Event* result = queue.back();
    queue.pop_back();

    return result;
}

void Event_queue::notify_event_handler()
{
    new_events.notify_all();
}

void Event_queue::event_handler_wait(unique_lock<mutex> * lck)
{
    new_events.wait(*lck);
}