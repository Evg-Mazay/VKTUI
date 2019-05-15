#include <vector>
#include <string>
#include <ctime>
#include <curl/curl.h>

#include "Network.h"
#include "Data_types.h"

int Network::main_loop()
{
    Message_data msg {
        0,          // message id
        time(NULL), // time
        255,        // sender id
        L"Привет!"   // text
    };

    backend->queue_push(Event(RECIEVED_MESSAGE, msg));
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

    return 0;
}
