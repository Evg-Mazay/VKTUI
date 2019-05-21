#ifndef __CURL_WRAPPER__
#define __CURL_WRAPPER__

#include <curl/curl.h>
#include <string>

#define LONGPOLL_ADDR(SERVER, KEY, TS) (std::string("https://") + SERVER +\
        "?act=a_check&key=" + KEY + "&ts=" + TS + "&wait=25&version=3")

#define AUTH_ADDR(user, pass) (std::string("https://oauth.vk.com/token") +\
        "?grant_type=password&client_id=3697615&client_secret=AlVXZFMUqyrnABp8ncuU&" +\
        "username=" + user + "&password=" + pass + "&v=5.95")

#define VK_ADDR(method, params, token) (std::string("https://api.vk.com/method/") +\
        method + "?" + params + "&v=5.95&access_token=" + token)

class Curl_wrapper
{
    CURL *curl;
    std::string buffer;

    static size_t write_data(void *ptr, size_t size, size_t nmemb, std::string* buffer);
    inline void set_addr(std::string addr);

public:
    Curl_wrapper();

    std::string* response();

    int get(std::string url);
    

};



#endif