#ifndef __BACKEND__
#define __BACKEND__

class Network;
class Frontend;
class Database;
class User_input;

class Backend
{
    Frontend* frontend;
    Database* database;
    Network* network;

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:
    int main_loop();
    
};


#endif