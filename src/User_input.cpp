#include <curses.h>
#include <string>

#include "User_input.h"
#include "Event.h"
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
        backend->queue_in_push(Event(EDIT_INPUT_MESSAGE, 
                                    event_factory.get_event_data_message(&text)));

        ch = getwc(stdin);

        if (ch == 27) //ESC
        {
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

    backend->queue_in_push(Event(SEND_INPUT_MESSAGE, 
                                    event_factory.get_event_data_message(text)));

    return 1;
}
