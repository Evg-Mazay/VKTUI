#ifndef __LONGPOLL__
#define __LONGPOLL__

#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>

#include "Data_types.h"
#include "Backend.h"
#include "classes/Curl_wrapper.h"

#define LONGPOLL_ADDR(SERVER, KEY, TS) (std::string("https://") + SERVER + "?act=a_check&key=" +\
												KEY + "&ts=" + TS + "&wait=25&version=3")

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

public:
    
    int main_loop();

    int set_credentials(credentials _user);
    int get_longpoll();
};



#endif