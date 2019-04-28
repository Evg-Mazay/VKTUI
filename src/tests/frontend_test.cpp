#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <string>
#include <thread>

#include "../Frontend.h"
#include "../User_input.h"

void input_loop(User_input* user_input)
{
    while (user_input->main_loop());
}

void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database)
{
    frontend->init_curses();
    user_input->frontend = frontend;
}

int main()
{
    // FILE* log = fopen("log.log", "w");
    // #define PRINT(msg) fprintf(log, msg);
    // #define PRINTF(format, msg) fprintf(log, format, msg);

    setlocale(LC_ALL, "");

    Frontend frontend;
    User_input user_input;
    init(NULL, &frontend, &user_input, NULL, NULL);
    thread user_input_thread(input_loop, &user_input);

    // frontend.print_debug_message(L"Лол");

    user_input_thread.join();

    // sleep(5);


    // fclose(log);
    return 0;
}