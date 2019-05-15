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


//-------
Network* network;
//-------


int online_test()
{
    return network->test_online();
}


//-------------------------------------------

void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database)
{
    std::vector<dialog> v;
    v.push_back((dialog){0, L"lol"});

    database->init_database("cache.db", v);
    network->set_credentials(database->get_credentials());
}

int main()
{
    setlocale(LC_ALL, "");

    network = new Network;
    Database database;
    init(network, NULL, NULL, NULL, &database);

    int err;

    printf("---------------------------\n");

    // auto start = clock();

    err = online_test();
    
    // auto end = clock();

    printf("RESULT: %s\n", network->get_buffer()->c_str());
    printf("RC: %d\n", err);

    // printf("_____\n|Time passed: %lu (%f seconds)|\n_____\n", 
    //                     end - start, (double) (end - start) / (double) CLOCKS_PER_SEC);

    return 0;
}