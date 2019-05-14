#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <ctime>

#include "User_input.h"
#include "Frontend.h"
#include "Database.h"
#include "Backend.h"
#include "Network.h"
#include "Data_types.h"
#include "classes/Event_queue.h"

int Backend::main_loop()
{
    std::unique_lock<std::mutex> lock(backend_main_mutex);
    new_event_queued.wait(lock);

    return process_queue();
}

void Backend::queue_push(Event event)
{
    queue.push(event);
    new_event_queued.notify_one();
}

int Backend::get_start_data()
{
    network->get_dialogs(&dialogs);
    frontend->print_dialogs(dialogs, selected_dialog);
    return 0;
}

void Backend::debug_print(std::wstring text, int mode)
{
    if (mode & DEBUG_PRINT_DISPLAY)
        frontend->print_debug_message(text);

    if (mode & DEBUG_PRINT_DATABASE)
        database->add_debug_message(text);

    if (mode & DEBUG_PRINT_LOG)
    {
        wofstream myfile("log.log", std::fstream::out | std::fstream::app);
        myfile.imbue(locale(""));
        myfile << text << L"\n";
        myfile.close();
    }
}

int Backend::process_queue()
{
    while (!queue.empty())
    {
        Event event = queue.pop();

        if (event.type == EXIT)
        {
            return 0;
        }
        else if (event.type == SEND_INPUT_MESSAGE)
        {
            Message_data msg = {
                0, 
                time(NULL), 
                dialogs[selected_dialog].id,
                *event.data.text_p
            };

            delete event.data.text_p;

            int error = database->add_message(dialogs[selected_dialog].id, msg);

            frontend->print_debug_message(database->last_error());
            if (!error)
                frontend->add_message(msg);
        }
        else if (event.type == EDIT_INPUT_MESSAGE)
        {
            frontend->show_input_text(*event.data.text_p);
        }
        else if (event.type == RESTORE_MESSAGES)
        {
            frontend->add_messages(database->restore_last_n_messages(
                        dialogs[selected_dialog].id, event.data.integer));
        }
        else if (event.type == KEY_PRESS)
        {
            if (event.data.integer == ARROW_LEFT)
            {
                selected_dialog = (selected_dialog + 1) % dialogs.size();

                frontend->reset_messages();
                frontend->add_messages(database->restore_last_n_messages(
                                                dialogs[selected_dialog].id, -1));

                frontend->print_dialogs(dialogs, selected_dialog);
            }
            else if (event.data.integer == ARROW_RIGHT)
            {
                selected_dialog = (selected_dialog - 1) % dialogs.size();

                frontend->reset_messages();
                frontend->add_messages(database->restore_last_n_messages(
                                                dialogs[selected_dialog].id, -1));

                frontend->print_dialogs(dialogs, selected_dialog);
            }
            else if (event.data.integer == ARROW_UP)
            {
                frontend->scroll_messages(-2);
            }
            else if (event.data.integer == ARROW_DOWN)
            {
                frontend->scroll_messages(2);
            }
        }
    }

    return 1;
}





