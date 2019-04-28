#ifndef __DATABASE__
#define __DATABASE__

#include <sqlite3.h>

class Network;
class Frontend;
class Backend;
class User_input;

class Database
{
    sqlite3* db_instance_read;
    sqlite3* db_instance_write;

    int init_database(const char* filename);
    int open_write(const char* filename);
    int open_read(const char* filename);

    int sql_result_parser(void*,int,char**,char**);

    friend void init(Network* network, Frontend* frontend, User_input* user_input,\
                        Backend* backend, Database* database);

public:
    int run_read(const char* command);
    int run_write(const char* command);
    
    void add_debug_message(const char* message);

    ~Database();
    
};


#endif