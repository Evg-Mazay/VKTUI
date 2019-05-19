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
Longpoll* longpoll;
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

void Backend::debug_print(std::wstring text, int mode)
{
    printf("%ls\n", text.c_str());
}

//-------------------------------------------

void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Longpoll* longpoll, Backend* backend, Database* database)
{
    std::vector<dialog> v;
    v.push_back((dialog){0, L"lol"});

    database->init_database("cache.db", v);
    longpoll->set_credentials(database->get_credentials());
}

int main()
{
    setlocale(LC_ALL, "");

    longpoll = new Longpoll;
    Database database;
    Backend backend;
    init(NULL, NULL, NULL, longpoll, &backend, &database);

    // int err = 0;

    printf("---------------------------\n");

    // auto start = clock();

    for (;;)
    {
        longpoll->main_loop();
    }
    
    // auto end = clock();

    // printf("%s\n", user.token.c_str());

    // printf("_____\n|Time passed: %lu (%f seconds)|\n_____\n", 
    //                     end - start, (double) (end - start) / (double) CLOCKS_PER_SEC);

    return 0;
}