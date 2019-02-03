#include <mutex>
#include <condition_variable>
#include <string>

#include "globals.h"
#include "renderer.h"
#include "state.h"

using namespace std;

Renderer::Renderer(State* _state, bool* _killswitch)
{
    state = _state;
    killswitch = _killswitch;
    init_curses();
}

Renderer::~Renderer()
{
    exit_curses();
}

void Renderer::init_curses()
{
    initscr();
    // keypad(stdscr, TRUE);
    noecho();
    // halfdelay(-1);
    //wtimeout(stdscr, 33);
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

    box(win_dialogs, 0 , 0);
    box(win_messages, 0 , 0);
    box(win_input, 0 , 0);
    box(win_debug, '*' , '*');

    mvprintw(LINES-1, 0, "PRESS ESC TO EXIT");
    refresh();

    wprintw(win_dialogs, "dialogs:\n\n");
    wprintw(win_messages, "messages:\n\n");
    wprintw(win_input, "input:\n\n");
    wprintw(win_debug, "debug:\n\n");

    wrefresh(win_dialogs);
    wrefresh(win_messages);
    wrefresh(win_input);
    wrefresh(win_debug);
}

void Renderer::exit_curses()
{
    delwin(win_dialogs);
    delwin(win_messages);
    delwin(win_input);
    delwin(win_debug);
    endwin();
}

void Renderer::print_debug_message(string text)
{
    wprintw(win_debug, "%s\n", text.c_str());
}


void Renderer::main_loop()
{
    unique_lock<mutex> lck(renderer_mutex);
    state->renderer_wait(&lck);

    string debug_message = state->get_debug_message();
    if (!debug_message.empty())
    {
        print_debug_message(state->get_debug_message());
        state->clear_debug_message();
    }

    wmove(win_input, 2,0);
    wprintw(win_input, "%s", state->get_input_text().c_str());

    wrefresh(win_debug);
    wrefresh(win_messages);
    wrefresh(win_input);
}




