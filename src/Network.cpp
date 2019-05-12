#include <vector>
#include <string>

#include "Network.h"
#include "classes/Event.h"

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

// при редактировании сообщения, прикольно было бы отображать предыдущие версии


/*
    Когда приходит сообщение, в struct Message_data запихивать
    только текст и всякие интежеры. Вложения и прочее передавать
    каким-нибудь интежером, по которому бэкенд может их САМ запросить
    у вк и САМ занести в БД. Network ничего писать в БД НЕ ДОЛЖЕН.
*/


/*
4 Новое сообщение
5 Редактирование сообщения

6, 7 Входящие/исходящие прочитаны

62 Пользователь набирает текст

*/