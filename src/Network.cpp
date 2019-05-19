#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <curl/curl.h>
#include <rapidjson/document.h>

#include "Network.h"
#include "Data_types.h"

int Network::main_loop()
{
    return 0; // сразу выйти
}


Network::Network()
{
    
}

int Network::get_dialogs(std::vector<dialog>* dialogs)
{
    dialogs->push_back((dialog){1, std::wstring(L"Антон")});
    dialogs->push_back((dialog){2, std::wstring(L"Не Антон")});
    dialogs->push_back((dialog){3, std::wstring(L"Тоже не Антон")});
    dialogs->push_back((dialog){4, std::wstring(L"Совсем не Антон")});

    return 0;
}

int Network::set_credentials(credentials _user)
{
    if (_user.id == 0)
        return 1;

    user = _user;
    return 0;
}

int Network::send_message(Message_data msg)
{
    return 0;
}


int Network::test_online()
{
    int err = curl.get(VK_ADDR("account.setOnline", "", user.token));

    if (err)
    {
        last_error.clear();
        last_error = L"CURL error: " + std::to_wstring(err);
        return 0;
    }

    rapidjson::Document d;
    d.Parse(curl.response()->c_str());

    if (d.HasMember("error"))
    {
        last_error.clear();
        last_error = L"VK error: " + std::to_wstring(err);
        return 0;
    }

    return 1;
}

std::wstring* Network::get_error_message()
{
    return &last_error;
}





