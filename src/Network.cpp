#include <vector>
#include <string>

#include "Network.h"
#include "Data_types.h"

int Network::main_loop()
{
    return 0;
}

int Network::get_dialogs(std::vector<dialog>* dialogs)
{
    dialogs->push_back((dialog){1, std::wstring(L"Антон")});
    dialogs->push_back((dialog){2, std::wstring(L"Не Антон")});
    dialogs->push_back((dialog){3, std::wstring(L"Тоже не Антон")});
    dialogs->push_back((dialog){4, std::wstring(L"Совсем не Антон")});

    return 0;
}

