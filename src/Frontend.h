#ifndef __FRONTEND__
#define __FRONTEND__

#include <curses.h>
#include <vector>

#include "User_input.h"
#include "classes/Event.h"

#define WIN_DIALOGS 1
#define WIN_MESSAGES 2
#define WIN_INPUT 4
#define WIN_DEBUG 8

using namespace std;

class Network;
class Backend;
class Database;
class User_input;

class Frontend
{
    Backend* backend;
    Database* database;

    WINDOW* win_messages;
    WINDOW* win_dialogs;
    WINDOW* win_input;
    WINDOW* win_debug;

    void init_curses();
    void exit_curses();

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    void print_debug_message(wstring text);
    void refresh_windows(const int WIN);
    void reset_windows(const int WIN);
    void show_input_text(wstring text);
    void clear_input();
    ~Frontend();

    void add_messages(vector<Message_data> messages);

    static void main_loop();
    
};



#endif