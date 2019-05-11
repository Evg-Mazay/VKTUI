#include <stdio.h>
#include <curses.h>
#include <mutex>
#include <condition_variable>
#include <string>

#include "User_input.h"
#include "Frontend.h"


void Frontend::refresh_windows(const int WIN)
{
    if (WIN & WIN_DIALOGS)
        wrefresh(win_dialogs);

    if (WIN & WIN_MESSAGES)
        win_messages->refresh();

    if (WIN & WIN_INPUT)
        wrefresh(win_input);

    if (WIN & WIN_DEBUG)
        wrefresh(win_debug);
}


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
    win_messages = new ScrollableWindow(messages_h-2, messages_w-2, messages_y+1, messages_x+1);
    win_input = newwin(input_h, input_w, input_y, input_x);
    win_debug = newwin(debug_h, debug_w, debug_y, debug_x);

    scrollok(win_debug, 1);

    attrset(WA_UNDERLINE);
    mvaddnstr(0,0,"VKTUI v0.0 \t PRESS ESC TO EXIT", COLS);
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
    refresh_windows(WIN_DIALOGS | WIN_MESSAGES | WIN_DEBUG | WIN_INPUT);

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

    refresh_windows(WIN_DIALOGS | WIN_MESSAGES | WIN_DEBUG | WIN_INPUT);
}

void Frontend::exit_curses()
{
    delwin(win_dialogs);
    delwin(win_messages->win());
    delwin(win_input);
    delwin(win_debug);
    endwin();
}

void Frontend::print_debug_message(wstring text)
{
    text += L"\n";
    waddwstr(win_debug, text.c_str());
    refresh_windows(WIN_DEBUG);
}

void Frontend::show_input_text(wstring text)
{
    werase(win_input);
    waddwstr(win_input, text.c_str());
    refresh_windows(WIN_INPUT);
}


void Frontend::add_message(Message_data message)
{
    // out_screen_buffer += message.text;
    // out_screen_messages.push_back(buffer_message_position{0,1,out_screen_buffer.length()});

    wattrset(win_messages->win(), WA_UNDERLINE);
    waddwstr(win_messages->win(), to_wstring(message.from).c_str());
    wattrset(win_messages->win(), 0);

    waddwstr(win_messages->win(), L": ");
    waddwstr(win_messages->win(), message.text.c_str());
    waddwstr(win_messages->win(), L"\n");

    win_messages->refresh();

    // refresh_windows(WIN_MESSAGES);
}

void Frontend::add_messages(std::vector<Message_data> messages)
{
    for(auto it = messages.begin(); it != messages.end(); ++it)
        add_message(*it);

}

void Frontend::scroll_messages(int n)
{
    win_messages->scrll(n);
    win_messages->refresh();
}

void Frontend::edit_message(int id, wstring new_text)
{
    
}





