#ifndef __NETWORK__
#define __NETWORK__

#include <vector>
#include <curl/curl.h>

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

    CURL *curl;
    std::string buffer;

    static size_t write_data(void *ptr, size_t size, size_t nmemb, std::string* buffer);

    inline std::string vk_addr(std::string method, std::string params);

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    int set_credentials(credentials _user);
    void set_addr(std::string addr);

    std::string* get_buffer();

    int test_online();

    int main_loop();
    int get_dialogs(std::vector<dialog>* dialogs);

    int send_message(Message_data msg);

    Network();
    
};


#endif