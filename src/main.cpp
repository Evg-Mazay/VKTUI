#include <stdio.h>
#include <curses.h>
#include <thread>
#include <unistd.h>
#include <locale.h>

#include "Network.h"
#include "Frontend.h"
#include "Backend.h"
#include "Database.h"
#include "classes/Event.h"

using namespace std;

void network_thread_main(Network* network)
{
    while (network->main_loop());
}

void input_thread_main(User_input* user_input)
{
    while (user_input->main_loop());
}

void backend_thread_main(Backend* backend)
{
    while (backend->main_loop());
}

void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database)
{
    setlocale(LC_ALL, "");

    network->backend = backend;

    frontend->backend = backend;
    frontend->database = database;
    frontend->init_curses();

    user_input->frontend = frontend;
    user_input->backend = backend;

    backend->network = network;
    backend->frontend = frontend;
    backend->database = database;

    database->init_database("cache.db");

    Event_data data;
    data.messages_count = -1;
    backend->queue_in_push(Event(RESTORE_MESSAGES, data));
}


int main(void)
{
    Network network;
    Frontend frontend;
    Backend backend;
    Database database;
    User_input user_input;

    init(&network, &frontend, &user_input, &backend, &database);

    thread input_thread(input_thread_main, &user_input);
    thread network_thread(network_thread_main, &network);
    thread backend_thread(backend_thread_main, &backend);

    input_thread.join();
    network_thread.join();
    backend_thread.join();

    return 0;
}