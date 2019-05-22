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
    std::lock_guard<std::mutex> lock(debug_print_mutex);

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
                0, 
                time(NULL), 
                dialogs[selected_dialog].id,
                *event.data.text_p
            };

            delete event.data.text_p;

            int id = network->send_message(msg);
            
            if (id == 0)
            {
                debug_print(network->get_error_message()->c_str(), DEBUG_PRINT_DISPLAY);
                continue;
            }
        }
        else if (event.type == RECIEVED_MESSAGE)
        {
            Message_data msg = *event.data.message_p;
            delete event.data.message_p;

            database->add_message(msg.person, msg);

            if (msg.person == dialogs[selected_dialog].id)
                frontend->add_message(msg, dialogs[selected_dialog]);

        }
        else if (event.type == EDIT_INPUT_MESSAGE)
        {
            frontend->show_input_text(*event.data.no_free_text_p);
        }
        else if (event.type == RESTORE_MESSAGES)
        {
            auto messages = database->restore_last_n_messages(
                        dialogs[selected_dialog].id, event.data.integer);
            frontend->add_messages(messages, dialogs[selected_dialog]);
        }
        else if (event.type == KEY_PRESS)
        {
            if (event.data.integer == ARROW_LEFT)
            {
                selected_dialog = (selected_dialog + 1) % dialogs.size();

                frontend->reset_messages();

                auto messages = database->restore_last_n_messages(
                                                dialogs[selected_dialog].id, -1);
                frontend->add_messages(messages, dialogs[selected_dialog]);

                frontend->print_dialogs(dialogs, selected_dialog);
            }
            else if (event.data.integer == ARROW_RIGHT)
            {
                selected_dialog = (selected_dialog + dialogs.size() - 1) % dialogs.size();

                frontend->reset_messages();
                
                auto messages = database->restore_last_n_messages(
                                                dialogs[selected_dialog].id, -1);
                frontend->add_messages(messages, dialogs[selected_dialog]);

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





