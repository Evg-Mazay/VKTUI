#include <stdio.h>
#include <curses.h>
#include <thread>
#include <unistd.h>
#include <locale.h>

#include "Network.h"
#include "Frontend.h"
#include "Backend.h"
#include "Database.h"

using namespace std;

void network_thread_main(Network* network)
{
    while (true)
        network->main_loop();
}

void frontend_thread_main(Frontend* frontend)
{
    while (true)
        frontend->main_loop();
}

void backend_thread_main(Backend* backend)
{
    while (true)
        backend->main_loop();
}

void init(Network* network, Frontend* frontend, \
            Backend* backend, Database* database)
{
    setlocale(LC_ALL, "");

    network->backend = backend;

    frontend->backend = backend;
    frontend->database = database;

    backend->network = network;
    backend->frontend = frontend;
    backend->database = database;

    frontend->init_curses();

    database->init_database("cache.db");
}


int main(void)
{
    Network network;
    Frontend frontend;
    Backend backend;
    Database database;

    init(&network, &frontend, &backend, &database);

    thread network_thread(network_thread_main, &network);
    thread frontend_thread(frontend_thread_main, &frontend);
    thread backend_thread(backend_thread_main, &backend);

    network_thread.join();
    frontend_thread.join();
    backend_thread.join();

    return 0;
}