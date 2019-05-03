#ifndef __BACKEND__
#define __BACKEND__

#include <vector>
#include <mutex>
#include <condition_variable>

#include "classes/Event.h"
#include "classes/Event_queue.h"

class Network;
class Frontend;
class Database;
class User_input;

class Backend
{
    Frontend* frontend;
    Database* database;
    Network* network;

    std::mutex backend_main_mutex;
    std::condition_variable new_event_queued;

    Event_queue queue_in;
    Event_queue queue_out;

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);
    void process_in_queue();
    void process_out_queue();

public:
    int main_loop();
    
    void queue_in_push(Event event);
    void queue_out_push(Event event);
};


#endif