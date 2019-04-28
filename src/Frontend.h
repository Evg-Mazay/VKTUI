#ifndef __FRONTEND__
#define __FRONTEND__

#include <curses.h>
#include <mutex>
#include <condition_variable>

#define WIN_DIALOGS 1
#define WIN_MESSAGES 2
#define WIN_INPUT 4
#define WIN_DEBUG 8

using namespace std;

class Network;
class Backend;
class Database;

class Frontend
{
    mutex wait_mutex;
    condition_variable ready_to_refresh;

    Backend* backend;
    Database* database;

    WINDOW* win_messages;
    WINDOW* win_dialogs;
    WINDOW* win_input;
    WINDOW* win_debug;

    void init_curses();
    void exit_curses();

    friend void init(Network* network, Frontend* frontend, \
                        Backend* backend, Database* database);

public:

    void print_debug_message(wstring text);

    void refresh();

    void main_loop();
    void reset_windows(const int WIN);
    ~Frontend();
    
};



#endif