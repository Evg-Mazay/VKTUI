#ifndef __FRONTEND__
#define __FRONTEND__

#include <curses.h>
#include <vector>

#include "User_input.h"
#include "classes/Event.h"
#include "classes/ScrollableWindow.h"

#define WIN_DIALOGS 1
#define WIN_MESSAGES 2
#define WIN_INPUT 4
#define WIN_DEBUG 8

using namespace std;

class Network;
class Backend;
class Database;
class User_input;



struct dialog
{
    int id;
    std::wstring name;
};


class Frontend
{
    Backend* backend;
    Database* database;

    ScrollableWindow* win_messages;
    WINDOW* win_dialogs;
    WINDOW* win_input;
    WINDOW* win_debug;

    long last_message_depth;

    void init_curses();
    void exit_curses();

    void messages_addwstr(wstring s);

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:

    void print_debug_message(wstring text);
    void refresh_windows(const int WIN);
    void show_input_text(wstring text);
    void clear_input();
    ~Frontend();

    void add_message(Message_data message);
    void add_messages(vector<Message_data> messages);
    void edit_message(int id, std::wstring new_text);

    void print_dialogs(std::vector<dialog> dialogs, int selected);

    void scroll_messages(int n);

    static void main_loop();
    
};



#endif