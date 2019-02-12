#include <curses.h>
#include <mutex>
#include <condition_variable>
#include <string>
#include <signal.h>

#include "globals.h"
#include "renderer.h"
#include "state.h"

using namespace std;

Renderer::Renderer(State* _state, bool* _killswitch)
{
    state = _state;
    killswitch = _killswitch;

    signal_processing_class = this;
    signal(SIGWINCH, WINCH_signal_handler);

    init_curses();
}

Renderer::~Renderer()
{
    exit_curses();
}

void Renderer::reset_windows(const int WIN)
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

void Renderer::resize_terminal()
{
    exit_curses();
    init_curses();
}

void WINCH_signal_handler(int signum)
{
    signal_processing_class->resize_terminal();
}

void Renderer::init_curses()
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

void Renderer::exit_curses()
{
    delwin(win_dialogs);
    delwin(win_messages);
    delwin(win_input);
    delwin(win_debug);
    endwin();
}

void Renderer::print_debug_message(wstring text)
{
    waddwstr(win_debug, text.c_str());
    waddwstr(win_debug, L"\n");
}


void Renderer::main_loop()
{
    unique_lock<mutex> lck(renderer_mutex);
    state->renderer_wait(&lck);

    wstring debug_message = state->pop_debug_message();
    while (!debug_message.empty())
    {
        print_debug_message(debug_message);
        debug_message = state->pop_debug_message();
    }

    wclrtoeol(win_input);
    wmove(win_input, 2,0);
    waddwstr(win_input, state->get_input_text().c_str());

    wrefresh(win_debug);
    wrefresh(win_input);
}




