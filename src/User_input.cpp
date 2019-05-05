#include <curses.h>
#include <string>

#include "User_input.h"
#include "classes/Event.h"
#include "Backend.h"

// return 0 - выход из программы
// return 1 - продолжить
int User_input::main_loop()
{
    // fflush(stdin);
    wchar_t ch = 0;
    text.clear();

    while (ch != 13) // ENTER
    {
        Event_data data{&text};
        backend->queue_in_push(Event(EDIT_INPUT_MESSAGE, data));

        ch = getwc(stdin);

        if (ch == 27) //ESC
        {
            backend->queue_in_push(Event(EXIT));
            return 0;
        }

        if (ch == 127) // BACKSPACE
        {
            if (!text.empty())
                text.pop_back();
            continue;
        }

        if (ch != 13)
            text += ch;
    }

    if (text.empty())
        return 1;

    Event_data data;
    data.message_data = new Message_data{0,1,23,3,wstring(text)};
    backend->queue_in_push(Event(SEND_INPUT_MESSAGE, data));

    return 1;
}
