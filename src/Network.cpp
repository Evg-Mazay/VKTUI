#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <curl/curl.h>

#include "Network.h"
#include "Data_types.h"

int Network::main_loop()
{
    return 0; // сразу выйти
}


size_t Network::write_data(void *ptr, size_t size, size_t nmemb, std::string* buffer)
{
    size_t real_size = size * nmemb;
    buffer->append((char*)ptr, real_size);
    return real_size;
}

void Network::set_addr(std::string addr)
{
    curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
}

Network::Network()
{
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
}

int Network::get_dialogs(std::vector<dialog>* dialogs)
{
    dialogs->push_back((dialog){1, std::wstring(L"Антон")});
    dialogs->push_back((dialog){2, std::wstring(L"Не Антон")});
    dialogs->push_back((dialog){3, std::wstring(L"Тоже не Антон")});
    dialogs->push_back((dialog){4, std::wstring(L"Совсем не Антон")});

    return 0;
}

std::string* Network::get_buffer()
{
    return &buffer;
}

int Network::set_credentials(credentials _user)
{
    if (_user.id == 0)
        return 1;

    user = _user;
    return 0;
}

inline std::string Network::vk_addr(std::string method, std::string params)
{
    return "https://api.vk.com/method/" + method +\
                      "?" + params + "&v=5.52&access_token=" + user.token;
}

int Network::send_message(Message_data msg)
{
    return 0;
}


int Network::test_online()
{
    set_addr(vk_addr("account.setOnline", ""));
    return curl_easy_perform(curl);
}





