#ifndef __STATE__
#define __STATE__

#include <mutex>
#include <condition_variable>
#include <string>

using namespace std;

class State
{
    bool* killswitch;

    mutex state_mutex;
    condition_variable state_changed;

    string input_text;
    string debug_message;
public:
    State(bool* _killswitch);
    void notify_renderer();
    void renderer_wait(unique_lock<mutex> * lck);

    string get_input_text();
    void clear_input_text();
    void append_to_input_text(char ch);

    void set_debug_message(string text);
    string get_debug_message();
    void clear_debug_message();
};

#endif