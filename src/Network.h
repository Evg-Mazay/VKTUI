#ifndef __NETWORK__
#define __NETWORK__

#include <vector>

#include "classes/Event.h"

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
    int main_loop();
    int get_dialogs(std::vector<dialog>* dialogs);
    
};


#endif