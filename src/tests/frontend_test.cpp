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
        frontend->print_debug_message(msg.text);\
        frontend->show_input_text(msg.text);\
        usleep(SLEEP);\
    }

using namespace std;

//----
Frontend* frontend;
//----

void message_edit_test()
{
    SEND_N_MESSAGES(10, L"loh", 0);
    sleep(1);
    frontend->edit_message(3, L"New text )))");
}

void scroll_test()
{
    SEND_N_MESSAGES(80, L"MESSAGE---------------------------------------", 0)

    sleep(1);

    frontend->scroll_messages(-2);

    sleep(1);

    frontend->scroll_messages(2);
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

void add_messages_slowly()
{
    SEND_N_MESSAGES(100, L"Message-----------------------------------------+--+--sdf", 1000000);
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

    auto start = clock();

    // message_edit_test();
    // scroll_test();
    add_messages_slowly();
    // stress_test();

    auto end = clock();

    printf("_____\n|Time passed: %lu (%f seconds)|\n_____\n", 
                        end - start, (double) (end - start) / (double) CLOCKS_PER_SEC);
    
    while (1);

    return 0;
}