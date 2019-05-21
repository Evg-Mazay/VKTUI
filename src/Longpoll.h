#ifndef __LONGPOLL__
#define __LONGPOLL__

#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>

#include "Data_types.h"
#include "Backend.h"
#include "classes/Curl_wrapper.h"


class Longpoll
{
    Backend* backend;
    Curl_wrapper curl;

    int got_server = 0;
    
    std::string key;
    std::string server;
    int ts = 0;

    credentials user = {0};

    void parse_updates();

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Longpoll* longpoll, Backend* backend, Database* database);

public:
    
    int main_loop();

    int set_credentials(credentials _user);
    int get_longpoll();
};



#endif