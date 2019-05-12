#ifndef __USER_INPUT__
#define __USER_INPUT__

#include <string>

#include "Frontend.h"
#include "Backend.h"
#include "classes/Event.h"

using namespace std;

#define ARROW_LEFT  0
#define ARROW_UP    1
#define ARROW_RIGHT 2
#define ARROW_DOWN  3

class Network;
class Database;

class User_input
{
    // enum {ESCAPE, TEST};

    Frontend* frontend; // frontend тут использовать только для дебага! (нет мьютексов).
    Backend* backend;

    wstring text;

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    int main_loop();
    
};

#endif