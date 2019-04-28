#include <curses.h>
#include <string>

#include "user_input.h"

// return 0 - выход из программы
// return 1 - продолжить
int User_input::main_loop()
{
    // fflush(stdin);
    wchar_t ch = 0;
    text.clear();

    while (ch != 13) // ENTER
    {
        frontend->show_input_text(text);

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

    frontend->print_debug_message(text);

    return 1;
}
