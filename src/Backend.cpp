#include <unistd.h>
#include <string>
#include <stdio.h>

#include "Backend.h"
#include "Frontend.h"
#include "Database.h"

int Backend::main_loop()
{
    sleep(1);

    frontend->print_debug_message(L"Привет, мир!");
    return 0;
    return 1;
}