#ifndef __DATABASE__
#define __DATABASE__

#include <sqlite3.h>
#include <codecvt>
#include <iostream>
#include <vector>

#include "classes/Event.h"

class Network;
class Frontend;
class Backend;
class User_input;

class Database
{
    sqlite3* db_instance_read;
    sqlite3* db_instance_write;

    char* errormesg = NULL;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    int init_database(const char* filename);
    int open_write(const char* filename);
    int open_read(const char* filename);

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:
    int run_read(const char* command, void* arg, int (*callback)
                                                        (void*, int, char**, char**));
    int run_write(const char* command, void* arg, int (*callback)
                                                        (void*, int, char**, char**));
    
    std::vector<Message_data> restore_last_X_messages(int X);
    void add_debug_message(const char* message); // сюда прикольно поставить триггер
    int add_message(int id, long int date, int from, int to, std::wstring text);

    std::wstring last_error();
    std::string to_utf8(std::wstring text);

    ~Database();
    
};


#endif