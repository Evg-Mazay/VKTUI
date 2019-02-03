
#include <string>

#include "state.h"

using namespace std;

State::State(bool* _killswitch)
{
    killswitch = _killswitch;
}


void State::set_debug_message(string text)
{
    lock_guard<mutex> lck(state_mutex);
    debug_message = text;
}

string State::get_debug_message()
{
    lock_guard<mutex> lck(state_mutex);
    return debug_message;
}

void State::clear_debug_message()
{
    lock_guard<mutex> lck(state_mutex);
    debug_message.clear();
}


void State::notify_renderer()
{
    state_changed.notify_all();
}

void State::renderer_wait(unique_lock<mutex> * lck)
{
    state_changed.wait(*lck);
}

void State::append_to_input_text(char ch)
{
    input_text += ch;
}

string State::get_input_text()
{
    return input_text;
}

void State::clear_input_text()
{
    input_text.clear();
}