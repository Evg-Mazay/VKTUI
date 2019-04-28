#include <stdio.h>
#include <sqlite3.h>
#include <string>

#include "Database.h"

using namespace std;

int Database::open_write(const char* filename)
{
    return sqlite3_open(filename, &db_instance_write);
}

int Database::open_read(const char* filename)
{
    return sqlite3_open_v2(filename, &db_instance_read, SQLITE_OPEN_READONLY, NULL);
}

Database::~Database()
{
    sqlite3_close_v2(db_instance_write);
    sqlite3_close(db_instance_read);
}

int Database::run_read(const char* command)
{
    return sqlite3_exec(db_instance_read, command, NULL, NULL, NULL);
}

int Database::run_write(const char* command)
{
    int error = sqlite3_exec(db_instance_write, command, NULL, NULL, NULL);
    // char* errormesg;
    // int error = sqlite3_exec(db_instance_write, command, NULL, NULL, &errormesg);
    // printf("error: %s\n", errormesg);
    // sqlite3_free(errormesg);
    return error;
}


int Database::sql_result_parser(void* null, int count, char** data, char** columns)
{

    // frontend->print_debug_message(new wstring(data[1]));
    return 0;
}

int Database::init_database(const char* filename)
{
    open_write(filename);
    run_write("CREATE TABLE debug_messages(\
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                    message TEXT);");

    // const char16_t str = u"Хай";
    wchar_t str[] = L"Хорошая строчка";

    add_debug_message((char *)str);


    /*
короче, надо делать sql_prepare16

    */

    open_read("cache.db");

    return 0;
}

void Database::add_debug_message(const char* message)
{
    string str("INSERT INTO debug_messages(message)");
    str.append("VALUES ('");
    str.append(message);
    str.append("');");

    // printf("%s\n", str.c_str());

    run_write(str.c_str());
}







