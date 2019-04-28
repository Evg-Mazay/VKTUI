#ifndef __USER_INPUT__
#define __USER_INPUT__

#include <string>

#include "Frontend.h"

using namespace std;

class Frontend;
class Network;
class Backend;
class Database;

class User_input
{
    enum {ESCAPE, TEST};

    Frontend* frontend;

    wstring text;

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    int main_loop();
    
};

#endif