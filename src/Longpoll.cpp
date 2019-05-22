#include <unistd.h>
#include <rapidjson/document.h>
#include <string>
#include <locale>
#include <codecvt>

#include "Longpoll.h"

int Longpoll::set_credentials(credentials _user)
{
    if (_user.id == 0)
        return 1;

    user = _user;
    return 0;
}

int Longpoll::get_longpoll()
{
    int err = curl.get(VK_ADDR("messages.getLongPollServer", "lp_version=5.65", user.token));
    
    if (err)
        return err;
    
    rapidjson::Document d;
    d.Parse(curl.response()->c_str());
    
    if (d.HasMember("error"))
    {
        return 1;
    }
    
    key = d["response"]["key"].GetString();
    server = d["response"]["server"].GetString();
    ts = d["response"]["ts"].GetInt();
    
    //printf("%s %s %d\n", key.c_str(), server.c_str(), ts);
    return 0;
}


void Longpoll::parse_updates()
{
    rapidjson::Document d;
    d.Parse(curl.response()->c_str());
    
    auto& a = d["updates"];
    for (unsigned i = 0; i < a.Size(); i++)
        if (a[i][0].GetInt() == LP_NEW_MESSAGE)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            
            Message_data msg;
            msg.id = a[i][1].GetInt();
            msg.flags = a[i][2].GetInt();
            msg.time = a[i][4].GetInt();
            msg.person = a[i][3].GetInt();
            msg.text = converter.from_bytes(a[i][5].GetString());
            // printf("%s\n", curl.response()->c_str());
            
            backend->queue_push(Event(RECIEVED_MESSAGE, msg));
        }
        else if (a[i][0].GetInt() == LP_USER_ONLINE)
        {
            backend->queue_push(Event(LP_USER_ONLINE, -a[i][1].GetInt()));
        }
        else if (a[i][0].GetInt() == LP_USER_OFFLINE)
        {
            backend->queue_push(Event(LP_USER_OFFLINE, -a[i][1].GetInt()));
        }
        else
        {
            backend->debug_print(L"Longpoll: " + std::to_wstring(a[i][0].GetInt()), DEBUG_PRINT_DISPLAY);
        }
}


int Longpoll::main_loop()
{
    if (!got_server)
    {
        backend->debug_print(L"Getting longpoll data...", DEBUG_PRINT_DISPLAY);
        if(get_longpoll())
        {
            backend->debug_print(L"Cant get longpoll data", DEBUG_PRINT_DISPLAY);
            sleep(3);
            return 1;
        }
        else
        {
            got_server = 1;
            backend->debug_print(L"Successfully got longpoll data!", DEBUG_PRINT_DISPLAY);
        }
    }
    
    
    curl.get(LONGPOLL_ADDR(server, key, std::to_string(ts)));
    
    rapidjson::Document d;
    d.Parse(curl.response()->c_str());
    
    //printf("DATA: %s", curl.response()->c_str());
    
    if (d.HasMember("failed") || d.HasMember("error"))
    {
        backend->debug_print(L"Longpoll query error", DEBUG_PRINT_DISPLAY);
        got_server = 0;
        return 1;
    }
    
    ts = d["ts"].GetInt();
    
    parse_updates();

    return 1;
}