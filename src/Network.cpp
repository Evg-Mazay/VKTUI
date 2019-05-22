#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <locale>
#include <codecvt>

#include "Network.h"
#include "Data_types.h"
#include "classes/Curl_wrapper.h"

int Network::main_loop()
{
    return 0; // сразу выйти
}

int Network::get_dialogs(std::vector<dialog>* dialogs)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    
    int err = curl.get(VK_ADDR("messages.getConversations", "extended=1", user.token));

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
    
    //printf("%s\n", curl.response()->c_str());
    
    auto& items = d["response"]["items"];
    auto& profiles = d["response"]["profiles"];
    for (unsigned i = 0; i < items.Size(); i++)
    {
        
        int id = items[i]["conversation"]["peer"]["id"].GetInt();
        if (id > 2000000000)
            continue;
        
        for (unsigned j = 0; j < profiles.Size(); ++j)
        {
            if (profiles[j]["id"] == id)
            {
                dialogs->push_back((dialog)
                            {id,
                            converter.from_bytes(profiles[j]["first_name"].GetString()),
                            converter.from_bytes(profiles[j]["last_name"].GetString()),
                            (bool) profiles[j]["online"].GetInt(),
                            });
                break;
            }
        }
    }
    
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
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    
    std::string params("user_id=");
    params += std::to_string(msg.person) + "&random_id=0&message=";
    params += std::string(curl_easy_escape(
                    &curl, converter.to_bytes(msg.text).c_str(), 0));

                                
    //printf("%s\n", params.c_str());
    
    int err = curl.get(VK_ADDR("messages.send", params.c_str(), user.token));
                                
    if (err)
    {
        last_error.clear();
        last_error = L"CURL error: " + std::to_wstring(err);
        return 0;
    }
    
    // printf("%s\n", curl.response()->c_str());

    rapidjson::Document d;
    d.Parse(curl.response()->c_str());
    
    
    if (d.HasMember("error"))
    {
        last_error = L"VK error: " + std::to_wstring(d["error"]["error_code"].GetInt());
        return 0;
    }
    
    
    int id = d["response"].GetInt();
    
    return id;
}

bool Network::test_online()
{
    int err = curl.get(VK_ADDR("account.setOnline", "", user.token));

    if (err)
    {
        last_error.clear();
        last_error = L"CURL error: " + std::to_wstring(err);
        return false;
    }

    rapidjson::Document d;
    d.Parse(curl.response()->c_str());

    if (d.HasMember("error"))
    {
        last_error.clear();
        last_error = L"VK error: " + std::to_wstring(err);
        return false;
    }

    return true;
}

std::wstring* Network::get_error_message()
{
    return &last_error;
}





