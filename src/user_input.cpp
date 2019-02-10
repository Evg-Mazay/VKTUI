#include <curses.h>
#include <string>

#include "user_input.h"
#include "classes/events.h"
#include "classes/messages.h"
#include "event_queue.h"

User_input::User_input(State* _state, Event_queue* _event_queue, bool* _killswitch)
{
    state = _state;
    killswitch = _killswitch;
    event_queue = _event_queue;
}

void User_input::main_loop()
{
    // fflush(stdin);
    wchar_t ch = 0;

    while (ch != 13) // ENTER
    {
        state->notify_renderer();

        ch = getwc(stdin);

        if (ch == 27) //ESC
        {
            *killswitch = true;
            return;
        }

        state->append_to_input_text(ch);
    }

    wstring message = state->get_input_text();
    message.pop_back();

    if (message[0] == 13)
    {
        state->clear_input_text();
        return;
    }
    if (message.empty())
        return;

    message = L"you wrote: " + message;

    state->push_debug_message(message);
    state->clear_input_text();
    state->notify_renderer();

}
