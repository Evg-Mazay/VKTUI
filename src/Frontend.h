#ifndef __FRONTEND__
#define __FRONTEND__

#include <curses.h>
#include <vector>


#include "Data_types.h"
#include "Longpoll.h"
#include "classes/ScrollableWindow.h"

using namespace std;

class Network;
class Backend;
class Database;
class User_input;

/*
 This class is responsible for drawing


 Frontend <-- Backend: what to draw
 Frontend --> Backend: where it was drawn (if needed)

*/

class Frontend
{
    Backend* backend;   // link to backend

    ScrollableWindow* win_messages;
    WINDOW* win_dialogs;
    WINDOW* win_input;
    WINDOW* win_debug;

    void init_curses(); // init curses, create and draw all windows
    void exit_curses(); // delete all windows and end curses

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Longpoll* longpoll, Backend* backend, Database* database);

public:

    void print_debug_message(wstring text); // print text in debug window
    void show_input_text(wstring text);     // replace text in input window
    void clear_input();                     // replace text in input window with nothing

    // replace text in dialogs window
    void print_dialogs(std::vector<dialog> dialogs, unsigned selected);
    
    ~Frontend();

    void add_message(Message_data message);             // print message in messages window
    void add_messages(vector<Message_data> messages);   // calls add_message in loop
    void reset_messages();                              // reset messages window

    // scroll messages n lines up (n < 0) or down (n > 0)
    void scroll_messages(int n);

    // !! пока работает не так, как задумано
    void edit_message(int id, std::wstring new_text); // edit specified message
    
};



#endif