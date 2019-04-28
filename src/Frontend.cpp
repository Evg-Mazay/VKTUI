#include <stdio.h>
#include <curses.h>
#include <mutex>
#include <condition_variable>
#include <string>

#include "Frontend.h"

void Frontend::main_loop()
{
    /*
    Тут не мешало бы разобраться с блокировкой, чтобы
    backend не мог че-то печатать, пока main_loop не
    заблокировался
    */
    unique_lock<mutex> lck(wait_mutex);
    ready_to_refresh.wait(lck);


    wrefresh(win_debug);

}

void Frontend::refresh()
{
    ready_to_refresh.notify_one();
}

Frontend::~Frontend()
{
    exit_curses();
}

void Frontend::reset_windows(const int WIN)
{

    if (WIN & WIN_DIALOGS)
    {
        werase(win_dialogs);
        box(win_dialogs, 0 , 0);
        wprintw(win_dialogs, "dialogs:\n\n");
        wrefresh(win_dialogs);
    }

    if (WIN & WIN_MESSAGES)
    {
        werase(win_messages);
        box(win_messages, 0 , 0);
        wprintw(win_messages, "messages:\n\n");
        wrefresh(win_messages);
    }

    if (WIN & WIN_INPUT)
    {
        werase(win_input);
        box(win_input, 0 , 0);
        wprintw(win_input, "input:\n\n");
        wrefresh(win_input);
    }

    if (WIN & WIN_DEBUG)
    {
        werase(win_debug);
        box(win_debug, '*' , '*');
        wprintw(win_debug, "debug:\n\n");
        wrefresh(win_debug);
    }
}


void Frontend::init_curses()
{
    initscr();
    // keypad(stdscr, TRUE);
    noecho();
    // halfdelay(-1);
    // wtimeout(stdscr, 33);
    // nodelay(stdscr, TRUE);
    // cbreak();

    int dialogs_h = LINES-2, dialogs_w = COLS/3;
    int dialogs_y = 0, dialogs_x = 0;
    win_dialogs = newwin(dialogs_h, dialogs_w, dialogs_y, dialogs_x);

    int messages_h = (LINES-2) - (LINES-2) * 0.25, messages_w = COLS/3;
    int messages_y = 0, messages_x = COLS/3;
    win_messages = newwin(messages_h, messages_w, messages_y, messages_x);

    int input_h = (LINES-2) - (LINES-2) * 0.75, input_w = COLS/3;
    int input_y = (LINES-2) - (LINES-2) * 0.25, input_x = COLS/3;
    win_input = newwin(input_h, input_w, input_y, input_x);

    int debug_h = LINES-2, debug_w = COLS/3;
    int debug_y = 0, debug_x = COLS*2/3;
    win_debug = newwin(debug_h, debug_w, debug_y, debug_x);

    mvprintw(LINES-1, 0, "PRESS ESC TO EXIT");
    refresh();

    reset_windows(WIN_DIALOGS | WIN_MESSAGES | WIN_INPUT | WIN_DEBUG);
}

void Frontend::exit_curses()
{
    delwin(win_dialogs);
    delwin(win_messages);
    delwin(win_input);
    delwin(win_debug);
    endwin();
}

void Frontend::print_debug_message(wstring text)
{
    waddwstr(win_debug, text.c_str());
    waddwstr(win_debug, L"\n");
}

