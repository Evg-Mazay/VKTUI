
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
    // lock_guard<mutex> lck(state_mutex);
    // state_mutex.lock();
    debug_messages.push_back(text);
    // state_mutex.unlock();
}

wstring State::pop_debug_message()
{
    // lock_guard<mutex> lck(state_mutex);
    // state_mutex.lock();
    if (debug_messages.empty())
        return wstring();

    wstring text = debug_messages.back();
    debug_messages.pop_back();
    // state_mutex.unlock();

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