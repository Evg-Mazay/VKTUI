#include <curses.h>
#include <string>

#include "User_input.h"
#include "classes/Event.h"
#include "Backend.h"

// return 0 - выход из программы
// return 1 - продолжить
int User_input::main_loop()
{
    wchar_t ch = 0;
    text.clear();

    while (ch != 13) // ENTER
    {
        Event_data data{&text};
        backend->queue_in_push(Event(EDIT_INPUT_MESSAGE, data));

        ch = getwc(stdin);

        if (ch == 27) //ESC
        {
            // Переписать

            wchar_t esc[3];

            // frontend->print_debug_message(L"ESC MODE... type 'qq' to exit");
            esc[0] = getwc(stdin);
            esc[1] = getwc(stdin);
            esc[2] = 0;

            fflush(stdin);


            Event_data key_data;
            if (esc[1] == L'D')
                key_data.key = ARROW_LEFT;
            else if (esc[1] == L'A')
                key_data.key = ARROW_UP;
            else if (esc[1] == L'C')
                key_data.key = ARROW_RIGHT;
            else if (esc[1] == L'B')
                key_data.key = ARROW_DOWN;
            else
                continue;

            backend->queue_in_push(Event(4, key_data));


            // frontend->print_debug_message(wstring(esc));

            if (esc[0] == 27 && esc[1] == 27)
            {
                backend->queue_in_push(Event(EXIT));
                return 0;
            }

            continue;
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
