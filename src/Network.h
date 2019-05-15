#ifndef __NETWORK__
#define __NETWORK__

#include <vector>

#include "Data_types.h"
#include "Backend.h"

class Frontend;
class Backend;
class Database;
class User_input;

class Network
{
    Backend* backend;

    credentials user = {0};

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    int set_credentials(credentials _user);

    int main_loop();
    int get_dialogs(std::vector<dialog>* dialogs);

    int send_message(Message_data msg);
    
};


#endif