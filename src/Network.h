#ifndef __NETWORK__
#define __NETWORK__


class Frontend;
class Backend;
class Database;

class Network
{
    Backend* backend;

    friend void init(Network* network, Frontend* frontend, \
                        Backend* backend, Database* database);
    
public:
    void main_loop();
    
};


#endif