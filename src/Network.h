#ifndef __NETWORK__
#define __NETWORK__

#include <vector>
#include <curl/curl.h>

#include "Data_types.h"
#include "Backend.h"
#include "classes/Curl_wrapper.h"

class Frontend;
class Backend;
class Database;
class User_input;
class Longpoll;

class Network
{
    Backend* backend;
    Curl_wrapper curl;

    std::wstring last_error;

    credentials user = {0};

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Longpoll* longpoll, Backend* backend, Database* database);

public:

    int set_credentials(credentials _user);
    void set_addr(std::string addr);

    std::wstring* get_error_message();

    int test_online();

    int main_loop();
    int get_dialogs(std::vector<dialog>* dialogs);

    int send_message(Message_data msg);

    Network();
    
};


#endif