#include <unistd.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <ctime>

#include "Frontend.h"
#include "Database.h"
#include "Backend.h"
#include "Network.h"
#include "classes/Event_queue.h"

int Backend::main_loop()
{
    std::unique_lock<std::mutex> lock(backend_main_mutex);
    new_event_queued.wait(lock);

    return process_in_queue() & process_out_queue();
}

int Backend::get_start_data()
{
    network->get_dialogs(&dialogs);
    frontend->print_dialogs(dialogs, selected_dialog);
    return 0;
}

int Backend::process_in_queue()
{
    while (!queue_in.empty())
    {
        Event event = queue_in.pop();

        int error;

        switch (event.type)
        {
            case EXIT:

                return 0;


            case SEND_INPUT_MESSAGE:
            
                error = database->add_message(
                    dialogs[selected_dialog].id,
                    0,
                    time(NULL),
                    23, 
                    24, 
                    event.data.message_data->text);

                frontend->print_debug_message(database->last_error());
                if (!error)
                    frontend->add_message(*event.data.message_data);

                delete event.data.message_data;
                break;


            case EDIT_INPUT_MESSAGE:

                frontend->show_input_text(*event.data.message_pointer);
                break;



            case RESTORE_MESSAGES:

                frontend->add_messages(database->restore_last_X_messages(
                            dialogs[selected_dialog].id, event.data.messages_count));
                break;

            case KEY_PRESS:

                if (event.data.key == ARROW_LEFT)
                {
                    selected_dialog = (selected_dialog + 1) % dialogs.size();

                    frontend->reset_messages();
                    frontend->add_messages(database->restore_last_X_messages(
                                                    dialogs[selected_dialog].id, -1));

                    frontend->print_dialogs(dialogs, selected_dialog);
                }
                else if (event.data.key == ARROW_RIGHT)
                {
                    selected_dialog = (selected_dialog - 1) % dialogs.size();

                    frontend->reset_messages();
                    frontend->add_messages(database->restore_last_X_messages(
                                                    dialogs[selected_dialog].id, -1));

                    frontend->print_dialogs(dialogs, selected_dialog);
                }
                else if (event.data.key == ARROW_UP)
                {
                    frontend->scroll_messages(-2);
                }
                else if (event.data.key == ARROW_DOWN)
                {
                    frontend->scroll_messages(2);
                }

                break;

        }
    }

    return 1;
}

int Backend::process_out_queue()
{
    return 1;
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




