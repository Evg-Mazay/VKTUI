
#include <string>
#include <thread>

#include "state.h"

using namespace std;

State::State(bool* _killswitch)
{
    killswitch = _killswitch;
}


void State::push_debug_message(wstring text)
{
    lock_guard<mutex> lck(state_mutex);
    debug_messages.push_back(text);
}

wstring State::pop_debug_message()
{
    lock_guard<mutex> lck(state_mutex);

    if (debug_messages.empty())
        return wstring(); // Здесь не анлочится, поэтому дедлок

    wstring text = debug_messages.back();

    debug_messages.pop_back();

    return text;
}


void State::notify_renderer()
{
    state_changed.notify_all();
}

void State::renderer_wait(unique_lock<mutex> * lck)
{
    state_changed.wait(*lck);
}

void State::append_to_input_text(wchar_t ch)
{
    input_text += ch;
}

wstring State::get_input_text()
{
    return input_text;
}

void State::clear_input_text()
{
    input_text.clear();
}

void State::backspace_in_input_text()
{
    if (!input_text.empty())
        input_text.pop_back();
}
