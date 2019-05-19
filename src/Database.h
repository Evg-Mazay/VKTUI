#ifndef __DATABASE__
#define __DATABASE__

#include <sqlite3.h>
#include <codecvt>
#include <iostream>
#include <vector>
#include <locale>

#include "Data_types.h"

class Network;
class Frontend;
class Backend;
class User_input;
class Longpoll;

/*
This class handles connection to database

Database <-- Backend: what to read/put into database
Database --> Backend: what was read or status code

*/

class Database
{
    sqlite3* db_instance_read;
    sqlite3* db_instance_write;

    char* errormesg = NULL;     // last error message

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; // database need utf8 string

    // open connections and create all_tables
    int init_database(const char* filename);
	int init_dialogs(std::vector<dialog> dialogs);

    // wrappers for sqlite3 open functions
    int open_write(const char* filename);
    int open_read(const char* filename);

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Longpoll* longpoll, Backend* backend, Database* database);

public:

    // run sql for reading 
    int run_read(const char* command, void* arg, int (*callback)
                                                        (void*, int, char**, char**));
    // run sql for writing
    int run_write(const char* command, void* arg, int (*callback)
                                                        (void*, int, char**, char**));

    // restore n messages from dialog (if n < 0: restore all)
    std::vector<Message_data> restore_last_n_messages(int dialog_id, int n);

    // add message into debug table
    void add_debug_message(std::wstring message); // сюда прикольно поставить триггер

    // add message into dialog
    int add_message(int dialog_id, int id, long int date,
                                        int person, std::wstring text);
    int add_message(int dialog_id, Message_data msg);

    // get last error message
    std::wstring last_error();

    // return user_id and token
    credentials get_credentials();

    // convert wstring to utf8
    std::string to_utf8(std::wstring text);

    ~Database();
    
};


#endif