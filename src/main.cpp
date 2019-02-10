#include <curses.h>
#include <thread>
#include <locale.h>

#include "renderer.h"
#include "state.h"
#include "user_input.h"
#include "event_handler.h"
#include "event_queue.h"
#include "B.h"

using namespace std;

void renderer_main(Renderer* renderer, bool* killswitch)
{
    while (! *killswitch)
        renderer->main_loop();
}

void user_input_main(User_input* user_input, bool* killswitch)
{
    while(! *killswitch)
        user_input->main_loop();

}

void event_handler_main(Event_handler* event_handler, bool* killswitch)
{
    while (! *killswitch)
        event_handler->main_loop();
}

void b_thread_main(B* b, bool* killswitch)
{
    while (! *killswitch)
        b->debug_generate_event();
}

int main(void)
{
    setlocale(LC_ALL, "");

    bool killswitch = false;

    State state(&killswitch);
    Event_queue event_queue;

    B b(&event_queue, &killswitch);
    Renderer renderer(&state, &killswitch);
    User_input user_input(&state, &event_queue, &killswitch);
    Event_handler event_handler(&state, &event_queue, &killswitch);

    thread b_thread(b_thread_main, &b, &killswitch);
    thread renderer_thread(renderer_main, &renderer, &killswitch);
    thread user_input_thread(user_input_main, &user_input, &killswitch);
    thread event_handler_thread(event_handler_main, &event_handler, &killswitch);

    b_thread.join();
    renderer_thread.join();
    user_input_thread.join();
    event_handler_thread.join();

    return 0;
}