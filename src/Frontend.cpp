#include <stdio.h>
#include <curses.h>
#include <mutex>
#include <condition_variable>
#include <string>
#include <unistd.h>

#include "User_input.h"
#include "Frontend.h"

Frontend::~Frontend()
{
    exit_curses();
}

void Frontend::init_curses()
{
    initscr();
    noecho();
    cbreak();
	
    // start_color();
    // init_pair(1, COLOR_YELLOW, COLOR_GREEN);

    int dialogs_h = LINES * 0.8;
    int dialogs_w = COLS * 0.4;
    int dialogs_x = 0;
    int dialogs_y = 1;

    int messages_h = LINES * 0.8;
    int messages_w = COLS * 0.6;
    int messages_x = COLS * 0.4;
    int messages_y = 1;

    int input_h = LINES * 0.2;
    int input_w = COLS * 0.4;
    int input_x = COLS * 0.6;
    int input_y = LINES * 0.8 + 1;

    int debug_h = LINES * 0.2;
    int debug_w = COLS * 0.6;
    int debug_x = 0;
    int debug_y = LINES * 0.8 + 1;

    win_dialogs = newwin(dialogs_h, dialogs_w, dialogs_y, dialogs_x);
    WINDOW* win_messages_dummy = newwin(messages_h, messages_w, messages_y, messages_x);
    win_messages = new ScrollableWindow( messages_h >= 20 ? messages_h * 10 : 200,
                                 messages_h-2, messages_w-2, messages_y+1, messages_x+1);
    win_input = newwin(input_h, input_w, input_y, input_x);
    win_debug = newwin(debug_h, debug_w, debug_y, debug_x);


    //keypad(win_input, 1);
    scrollok(win_debug, TRUE);

    attrset(WA_UNDERLINE);
    mvaddnstr(0,0,"VKTUI v0.0 \t PRESS Ctrl+C TO EXIT", COLS);
    attrset(0);
    refresh();

    box(win_dialogs, 0, 0);
    mvwprintw(win_dialogs, 0, 0, "Dialogs");
    box(win_messages_dummy, 0, 0);
    mvwprintw(win_messages_dummy, 0, 0, "Messages");
    box(win_input, 0, 0);
    mvwprintw(win_input, 0, 0, "Input");
    box(win_debug, 0, 0);
    mvwprintw(win_debug, 0, 0, "Debug");

    wrefresh(win_messages_dummy);
    wrefresh(win_dialogs);
    wrefresh(win_input);
    wrefresh(win_debug);

    wresize(win_dialogs, dialogs_h - 2, dialogs_w - 2);
    wresize(win_messages_dummy, messages_h - 2, messages_w - 2);
    wresize(win_input, input_h - 2, input_w - 2);
    wresize(win_debug, debug_h - 2, debug_w - 2);
    mvwin(win_dialogs, dialogs_y + 1, dialogs_x + 1);
    mvwin(win_messages_dummy, messages_y + 1, messages_x + 1);
    mvwin(win_input, input_y + 1, input_x + 1);
    mvwin(win_debug, debug_y + 1, debug_x + 1);

    werase(win_dialogs);
    werase(win_messages_dummy);
    werase(win_input);
    werase(win_debug);

    delwin(win_messages_dummy);

    win_messages->refresh();
    wrefresh(win_dialogs);
    wrefresh(win_input);
    wrefresh(win_debug);
}

void Frontend::exit_curses()
{
    delwin(win_dialogs);
    win_messages->del();
    delwin(win_input);
    delwin(win_debug);
    endwin();
}

void Frontend::print_debug_message(wstring text)
{
    waddwstr(win_debug, L"\n");
    waddwstr(win_debug, text.c_str());
    wrefresh(win_debug);
}

void Frontend::show_input_text(wstring text)
{
    werase(win_input);
    waddwstr(win_input, text.c_str());
    wrefresh(win_input);
}


void Frontend::add_message(Message_data message, dialog _dialog)
{
    //print_debug_message(to_wstring(_dialog.id) + L" " + to_wstring(message.person));
    win_messages->print(L"✓ ");

    win_messages->attr(WA_UNDERLINE);
    if (message.flags & 2)
        win_messages->print(L"Я");
    else
        win_messages->print(_dialog.first_name);
    win_messages->attr(0);

    win_messages->print(L": ");
    win_messages->print(message.text.c_str());
    win_messages->print(L"\n");

    win_messages->refresh();

    // refresh_windows(WIN_MESSAGES);
}

void Frontend::add_messages(std::vector<Message_data> messages, dialog _dialog)
{
    for(auto it = messages.begin(); it != messages.end(); ++it)
        add_message(*it, _dialog);
}

void Frontend::scroll_messages(int n)
{
    win_messages->scrll(n);
    win_messages->refresh();
}

void Frontend::edit_message(int depth, wstring new_text)
{
    win_messages->edit(new_text, depth);
}

void Frontend::print_dialogs(std::vector<dialog> dialogs, unsigned selected)
{
    werase(win_dialogs);
    wmove(win_dialogs,0,0);

    for (unsigned i = 0; i < dialogs.size() && i < 20; ++i)
    {
        if (i == selected)
            wattron(win_dialogs, A_REVERSE);

        if (dialogs[i].online)
            waddwstr(win_dialogs, L"● ");
        else
            waddwstr(win_dialogs, L"○ ");

        waddwstr(win_dialogs, (dialogs[i].first_name + L" "+dialogs[i].last_name).c_str());

        wattron(win_dialogs, A_BLINK | A_BOLD);
        waddwstr(win_dialogs, dialogs[i].icon.c_str());
        wattroff(win_dialogs, A_BLINK | A_BOLD);

        waddwstr(win_dialogs, L"\n");

        wattrset(win_dialogs, 0);
    }

    wrefresh(win_dialogs);
}

void Frontend::reset_messages()
{
    win_messages->reset();
    win_messages->refresh();
}


void Frontend::set_cursor(int state)
{
    curs_set(state);
}

void Frontend::do_beep()
{
    beep();
}




