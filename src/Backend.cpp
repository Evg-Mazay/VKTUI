#include <unistd.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "Backend.h"
#include "Frontend.h"
#include "Database.h"
#include "classes/Event_queue.h"

int Backend::main_loop()
{
    std::unique_lock<std::mutex> lock(backend_main_mutex);
    new_event_queued.wait(lock);

    process_in_queue();
    process_out_queue();

    return 1;
}

void Backend::process_in_queue()
{
    while (!queue_in.empty())
    {
        Event event = queue_in.pop();

        switch (event.get_type())
        {
            case SEND_INPUT_MESSAGE:
            
                frontend->print_debug_message(*event.get_data().message_data.text);
                delete event.get_data().message_data.text;
                break;

            case EDIT_INPUT_MESSAGE:

                frontend->show_input_text(*event.get_data().message_data.text);
                break;
        }
    }
}

void Backend::process_out_queue()
{

}

void Backend::queue_in_push(Event event)
{
    queue_in.push(event);
    new_event_queued.notify_one();
}
void Backend::queue_out_push(Event event)
{
    queue_out.push(event);
    new_event_queued.notify_one();
}




