#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <string>
#include <thread>
#include <ctime>

#include "../Frontend.h"

#define SEND_N_MESSAGES(N, TEXT, SLEEP) Message_data msg{0,1,2,3};\
    for (int i = 0; i < N; ++i)\
    {\
        msg.id = i;\
        msg.text = TEXT + to_wstring(i);\
        frontend->add_message(msg);\
        usleep(SLEEP);\
    }

using namespace std;

//----
Frontend* frontend;
//----

void message_edit_test()
{
    SEND_N_MESSAGES(100, L"loh", 0);
    sleep(1);
    frontend->edit_message(3, L"New text )))");

    frontend->add_message(msg);
    frontend->add_message(msg);
    frontend->add_message(msg);

    frontend->scroll_messages(-100);
}

void add_messages_slowly()
{
    SEND_N_MESSAGES(1000, L"MESSAGE---------------------------------------", 1000000)
}

void stress_test()
{
    Message_data msg{0,1,2,3};
    for (int i = 1; i < 50000; ++i)
    {
        msg.text = L"Message-------------------------------- " + to_wstring(i);
        frontend->add_message(msg);
        frontend->print_debug_message(msg.text);
        frontend->show_input_text(msg.text);
    }
}

void scroll_test()
{
    SEND_N_MESSAGES(365, L"Message-------------------", 20000);

    for (int i = 0; i < 310; ++i)
    {
        frontend->scroll_messages(-1);
        usleep(20000);
    }

    // for (int i = 0; i < 10; ++i)
    // {
    //     msg.id = i;
    //     msg.text = L"TEXT" + to_wstring(i);
    //     frontend->add_message(msg);
    //     usleep(200000);
    // }
}


//-------------------------------------------

void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database)
{
    frontend->init_curses();
}

int main()
{
    setlocale(LC_ALL, "");

    frontend = new Frontend;
    init(NULL, frontend, NULL, NULL, NULL);

    // auto start = clock();

    message_edit_test();
    // scroll_test();
    // add_messages_slowly();
    // stress_test();

    // auto end = clock();

    // printf("_____\n|Time passed: %lu (%f seconds)|\n_____\n", 
    //                     end - start, (double) (end - start) / (double) CLOCKS_PER_SEC);
    
    while (1);

    return 0;
}