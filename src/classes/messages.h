#ifndef __MESSAGES__
#define __MESSAGES__

#include <string>

using namespace std;

class Message
{
    int id;
    string text;
public:
    Message(int _id, string _text);
    int get_id();
    string get_text();
    
};

#endif