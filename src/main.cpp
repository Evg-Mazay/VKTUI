#include <stdio.h>
#include <curses.h>
#include <thread>
#include <unistd.h>
#include <locale.h>

#include "User_input.h"
#include "Network.h"
#include "Frontend.h"
#include "Backend.h"
#include "Database.h"
#include "Data_types.h"

using namespace std;

// longpoll thread
void network_thread_main(Network* network)
{
    while (network->main_loop());
}

// keyboard-blockable thread
void input_thread_main(User_input* user_input)
{
    while (user_input->main_loop());
}

// main thread
void backend_thread_main(Backend* backend)
{
    while (backend->main_loop());
}

// prepares program for ready-to run state
void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database)
{
    setlocale(LC_ALL, "");

    frontend->backend = backend;
    frontend->init_curses();

    user_input->backend = backend;

    backend->network = network;
    backend->frontend = frontend;
    backend->database = database;
    backend->get_start_data();

    database->init_database("cache.db", backend->dialogs);

    network->backend = backend;
    network->set_credentials(database->get_credentials());

    backend->queue_push(Event(RESTORE_MESSAGES, -1));
    backend->process_queue();

}

// initialization and threading
int main(void)
{
    Network network;
    Frontend frontend;
    Backend backend;
    Database database;
    User_input user_input;

    init(&network, &frontend, &user_input, &backend, &database);

    thread input_thread(input_thread_main, &user_input);
    thread backend_thread(backend_thread_main, &backend);
    thread network_thread(network_thread_main, &network);

    input_thread.join();
    network_thread.join();
    backend_thread.join();

    return 0;
}