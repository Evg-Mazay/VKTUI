#ifndef __NETWORK__
#define __NETWORK__


class Frontend;
class Backend;
class Database;
class User_input;

class Network
{
    Backend* backend;

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);
    
public:
    void main_loop();
    
};


#endif