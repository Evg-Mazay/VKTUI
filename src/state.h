#ifndef __STATE__
#define __STATE__

#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>

using namespace std;

class State
{
    bool* killswitch;

    mutex state_mutex;
    condition_variable state_changed;

    wstring input_text;
    vector<wstring> debug_messages;
public:
    State(bool* _killswitch);
    void notify_renderer();
    void renderer_wait(unique_lock<mutex> * lck);

    wstring get_input_text();
    void clear_input_text();
    void append_to_input_text(wchar_t ch);

    void push_debug_message(wstring text);
    wstring pop_debug_message();
};

#endif