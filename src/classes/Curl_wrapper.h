#ifndef __CURL_WRAPPER__
#define __CURL_WRAPPER__

#include <curl/curl.h>
#include <string>


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