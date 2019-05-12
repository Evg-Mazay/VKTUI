#include <sqlite3.h>
#include <string>
#include <cstring>
#include <codecvt>
#include <iostream>
#include <stdlib.h>
#include <locale>
#include <vector>

#include "Database.h"
#include "classes/Event.h"

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

int Database::run_read(const char* command, void* arg, int (*callback)
                                                        (void*, int, char**, char**))
{
    return sqlite3_exec(db_instance_read, command, callback, arg, &errormesg);
}

int Database::run_write(const char* command, void* arg, int (*callback)
                                                        (void*, int, char**, char**))
{
    if (errormesg)
        sqlite3_free(errormesg);

    int error = sqlite3_exec(db_instance_write, command, callback, arg, &errormesg);
    return error;
}


int Database::init_database(const char* filename, std::vector<dialog> dialogs)
{
    open_write(filename);
    run_write("CREATE TABLE debug_messages (\
                    ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                    message TEXT);", NULL, NULL);

    for (int i = 0; i < dialogs.size(); ++i)
    {
        auto str = string("CREATE TABLE messages" + to_string(dialogs[i].id) + " (\
                    'id' INTEGER,\
                    'date' DATE,\
                    'from' INTEGER,\
                    'to' INTEGER,\
                    'text' TEXT);");
        run_write(str.c_str(), NULL, NULL);
    }
    

    open_read(filename);

    return 0;
}

void Database::add_debug_message(const char* message)
{
    string str("INSERT INTO debug_messages(message)");
    str.append("VALUES ('");
    str.append(message);
    str.append("');");

    run_write(str.c_str(), NULL, NULL);
}

int Database::add_message(int dialog_id, int id, long int date, 
                                        int from, int to, wstring text)
{
    // НЕ ПОДДЕРЖИВАЕТ КАВЫЧКИ В ТЕКСТЕ
    string str("INSERT INTO messages" + to_string(dialog_id) + " VALUES (" +
        to_string(id)           + "," +
        to_string(date)         + "," +
        to_string(from)         + "," +
        to_string(to)           + "," +
        "'" + to_utf8(text) + "'" + ");");

    return run_write(str.c_str(), NULL, NULL);
}

vector<Message_data> Database::restore_last_X_messages(int dialog_id, int X)
{
    string str("SELECT [id], [date], [from], [to], [text] FROM \
        messages" + to_string(dialog_id) + " ORDER BY date LIMIT " + to_string(X) + ";");

    vector<Message_data> vec;

    auto callback = [](void* vec, int count, char** data, char** columns) -> int
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        
        Message_data msg{
            atoi(data[0]),
            atoi(data[1]),
            atoi(data[2]),
            atoi(data[3]),
            converter.from_bytes(data[4])
        };
        
        ((vector<Message_data>*)vec)->push_back(msg);
        return 0;
    };

    run_read(str.c_str(), (void *) &vec, callback);
    
    return vec;
}


string Database::to_utf8(wstring text)
{
    string converted = converter.to_bytes(text);
    return converted;
}

wstring Database::last_error()
{
    if (errormesg)
        return wstring(errormesg, errormesg + std::strlen(errormesg));
    else
        return wstring(L"OK");
}






