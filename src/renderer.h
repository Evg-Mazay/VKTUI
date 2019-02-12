#ifndef __RENDERER__
#define __RENDERER__

#include <curses.h>
#include <mutex>

using namespace std;

class State;
class Renderer;

static Renderer* signal_processing_class;
void WINCH_signal_handler(int signum);

class Renderer
{
    bool* killswitch;

    mutex renderer_mutex;
    State* state;

    WINDOW* win_messages;
    WINDOW* win_dialogs;
    WINDOW* win_input;
    WINDOW* win_debug;

    void init_curses();
    void exit_curses();

public:
    Renderer(State* _state, bool* _killswitch);
    ~Renderer();

    void resize_terminal();

    void reset_windows(const int WIN);

    void print_debug_message(wstring text);

    void main_loop();
    
};

#endif