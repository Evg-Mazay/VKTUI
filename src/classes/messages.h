#ifndef __MESSAGES__
#define __MESSAGES__

#include <string>

using namespace std;

class Message
{
    int id;
    wstring text;
public:
    Message(int _id, wstring _text);
    int get_id();
    wstring get_text();
    
};

#endif