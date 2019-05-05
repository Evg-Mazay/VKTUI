#ifndef __USER_INPUT__
#define __USER_INPUT__

#include <string>

#include "Frontend.h"
#include "Backend.h"
#include "classes/Event.h"

using namespace std;

class Network;
class Database;

class User_input
{
    enum {ESCAPE, TEST};

    Frontend* frontend;
    Backend* backend;

    wstring text;

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    int main_loop();
    
};

#endif