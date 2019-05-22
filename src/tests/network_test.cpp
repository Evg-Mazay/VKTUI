#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <string>
#include <thread>
#include <ctime>
#include <vector>

#include "../Data_types.h"
#include "../Database.h"
#include "../Network.h"
#include "../Longpoll.h"


//-------
Network* network;
//-------

void Backend::queue_push(Event event)
{
    printf("%d %ld %d %ls\n",
        event.data.message_p->id,
        event.data.message_p->time,
        event.data.message_p->person,
        event.data.message_p->text.c_str()
    );
}

int online_test()
{
    return network->test_online();
}

int dialogs_test()
{
    std::vector<dialog> dialogs;
    
    network->get_dialogs(&dialogs);
    
    for (unsigned i = 0; i < dialogs.size(); ++i)
        printf("%d %ls\n", dialogs[i].id, dialogs[i].first_name.c_str());
    
    return 0;
}

int message_send_test()
{
    Message_data msg {
        0,
        0,
        time(NULL),
        23877122,
        L"ТЕСТ месседж"
    };
    
    int id = network->send_message(msg);
    
    msg.id = id;
    //backend->queue_push(Event(RECIEVED_MESSAGE, msg));
    
    return id;
}


//-------------------------------------------

void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Longpoll* longpoll, Backend* backend, Database* database)
{
    database->init_database("cache.db");
    network->set_credentials(database->get_credentials());
}

int main()
{
    setlocale(LC_ALL, "");

    network = new Network;
    Database database;
    Backend backend;
    init(network, NULL, NULL, NULL, &backend, &database);

    int err = 0;

    printf("---------------------------\n");

    // auto start = clock();

    err = dialogs_test();
    //err = message_send_test();
    //err = online_test();
    
    // auto end = clock();

    printf("LAST_ERR: %ls\n", network->get_error_message()->c_str());
    printf("RC: %d\n", err);
    

    // printf("_____\n|Time passed: %lu (%f seconds)|\n_____\n", 
    //                     end - start, (double) (end - start) / (double) CLOCKS_PER_SEC);

    return 0;
}