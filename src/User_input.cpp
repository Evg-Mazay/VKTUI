#include <curses.h>
#include <string>

#include "User_input.h"
#include "Data_types.h"
#include "Backend.h"


// return 0 - выход из программы
// return 1 - продолжить
int User_input::main_loop()
{
    wchar_t ch = 0;
    text.clear();

    while (ch != 13) // ENTER
    {
        backend->queue_push(Event(EDIT_INPUT_MESSAGE, &text));

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
                key_data.integer = ARROW_LEFT;
            else if (esc[1] == L'A')
                key_data.integer = ARROW_UP;
            else if (esc[1] == L'C')
                key_data.integer = ARROW_RIGHT;
            else if (esc[1] == L'B')
                key_data.integer = ARROW_DOWN;
            else
                continue;

            backend->queue_push(Event(4, key_data));


            // frontend->print_debug_message(wstring(esc));

            if (esc[0] == 27 && esc[1] == 27)
            {
                backend->queue_push(Event(EXIT));
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

    backend->queue_push(Event(SEND_INPUT_MESSAGE, text));

    return 1;
}
