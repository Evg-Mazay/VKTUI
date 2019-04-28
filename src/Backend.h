#ifndef __BACKEND__
#define __BACKEND__

class Network;
class Frontend;
class Database;

class Backend
{
    Frontend* frontend;
    Database* database;
    Network* network;

    friend void init(Network* network, Frontend* frontend, \
                        Backend* backend, Database* database);

public:
    void main_loop();
    
};


#endif