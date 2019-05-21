#ifndef __AUTH_INTERFACE__
#define __AUTH_INTERFACE__

#include <curses.h>
#include <string>
#include <rapidjson/document.h>

#include "../Data_types.h"
#include "Curl_wrapper.h"

class Auth_interface
{
    Curl_wrapper curl;

    int id = 0;
    std::string token;

    WINDOW* username_win;
    WINDOW* password_win;
    WINDOW* info_win;

    std::string error_text;

    std::string username_text;
    std::string password_text;

    const char* logo_1x10 = "VKTUI v0.0";

    const char* logo_18x32 = "\
  ...   ..   ..   ..  ...  ...  \
.. .... ........................\
...  ...  ...  ...  ...  ...  ..\
  ...   ..   ..   ..  ...  ...  \
#######.  .######## ...  #######\
 #######..   ######.  ..########\
  #######.   ######.  .######## \
...#######...###### ..#######...\
  ..########################..  \
...  :######################. ..\
  ... .#######################. \
  ...   .##########: .#########.\
...  ...  ..#######.... ########\
  ...   ..   ..   ..  ...  ...  \
...  ...  ...  ...  ...  ...  ..\
  ...   ..   ..   ..  ...  ...  \
                                \
           VKTUI v0.0           \
";


public:
    Auth_interface();
    ~Auth_interface();

    bool web_request();
    credentials get();
    
};




#endif