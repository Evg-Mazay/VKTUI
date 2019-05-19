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

int Network::main_loop()
{
    return 0; // сразу выйти
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
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	
	std::string params("user_id=");
	params += std::to_string(msg.person) + "&random_id=0&message=";
	params += std::string(curl_easy_escape(
					&curl, converter.to_bytes(msg.text).c_str(), 0));

								
	printf("%s\n", params.c_str());
	
	int err = curl.get(VK_ADDR("messages.send", params.c_str(), user.token));
								
	if (err)
	{
		last_error.clear();
        last_error = L"CURL error: " + std::to_wstring(err);
        return 0;
	}
	
	printf("%s\n", curl.response()->c_str());
								
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





