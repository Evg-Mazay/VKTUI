#ifndef __BACKEND__
#define __BACKEND__

#include <vector>
#include <mutex>
#include <condition_variable>

#include "Data_types.h"
#include "classes/Event_queue.h"

class Network;
class Frontend;
class Database;
class User_input;

/*
This class is responsible for parsing Events from User_input / Network,
sending data to Database and Frontend

main_loop() works in backend thread and sleeps until new event is pushed to queue 



                 Events like                Events like
┌──────────┐    "Key pressed"             "Message added"   ┌───────┐
│User_input│─────────────────────┐   ┌──────────────────────│Network│
└──────────┘                     ▼   ▼                      └───────┘
                                ┌─────┐                         ▲
                                │queue│                         │
 ┌────────┐   "draw something" ╔┴═════┴╗     "send message"     │
 │Frontend│◀───────────────────║Backend║────────────────────────┘
 └────────┘                    ╚═══════╝
                                   │
                                   │ "read data"
                                   │ "write data"
                                   │
                                   ▼ 
                               ┌────────┐
                               │Database│
                               └────────┘

*/

class Backend
{
    Frontend* frontend; //link to other components
    Database* database;
    Network* network;

    std::mutex backend_main_mutex;              // mutex for condition variable
    std::condition_variable new_event_queued;   // holding main_loop

    Event_queue queue;  // incoming events go here

    std::vector<dialog> dialogs; // dialog list
    int selected_dialog = 0;

    int process_queue();

    // initialization
    int get_start_data();

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:
    // sleeps until condition variable. If returns 0, thread dies.
    int main_loop();
    
    // add new event into queue (releases condition variable)
    void queue_push(Event event);
};


#endif