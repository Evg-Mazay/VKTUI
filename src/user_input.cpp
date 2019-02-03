#include <stdio.h>
#include <curses.h>

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
    char ch = 0;

    while (ch != '\n')
    {
        ch = getch();

        if (ch == 27) //ESC
        {
            *killswitch = true;
            return;
        }

        state->append_to_input_text(ch);
        state->notify_renderer();
    }



    Message* msg = new Message(0, "your message: " + state->get_input_text());
    Event* event = new Event_new_message(msg);
    event_queue->push(event);

    state->clear_input_text();

}