#include <curses.h>

#include "Auth_interface.h"

Auth_interface::Auth_interface()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    int username_win_h = 3;
    int username_win_w = COLS/1.5;
    int username_win_y = LINES * 0.6;
    int username_win_x = COLS/2 - username_win_w/2;

    int d = LINES/10 < 3 ? 3 : LINES/10; // input's y offset

    int password_win_h = username_win_h;
    int password_win_w = username_win_w;
    int password_win_y = username_win_y + d;
    int password_win_x = username_win_x;

    box(stdscr, 0, 0);
    refresh();

    if (LINES < 34 || COLS < 40)
    {
        int y = LINES * 0.4 - 1/2;
        int x = COLS/2 - 10/2;

        WINDOW* logo_win = newwin(1, 10, y, x);
        wprintw(logo_win, logo_1x10);
        wrefresh(logo_win);
        delwin(logo_win);
    }
    else
    {
        int y = LINES * 0.35 - 18/2;
        int x = COLS/2 - 32/2;

        WINDOW* logo_win = newwin(18, 32, y, x);
        wprintw(logo_win, logo_18x32);
        wrefresh(logo_win);
        delwin(logo_win);
    }

    username_win = newwin(username_win_h, username_win_w, username_win_y, username_win_x);
    password_win = newwin(password_win_h, password_win_w, password_win_y, password_win_x);
    info_win = newwin(password_win_h - 1, password_win_w,
                                                      password_win_y + d, password_win_x);

    box(username_win, 0, 0);
    box(password_win, 0, 0);
    mvwprintw(username_win, 0, 0, "Username");
    mvwprintw(password_win, 0, 0, "Password");
    wrefresh(username_win);
    wrefresh(password_win);

    wresize(username_win, username_win_h - 2, username_win_w - 2);
    wresize(password_win, password_win_h - 2, password_win_w - 2);
    mvwin(username_win, username_win_y + 1, username_win_x + 1);
    mvwin(password_win, password_win_y + 1, password_win_x + 1);

    werase(username_win);
    werase(password_win);
    wrefresh(password_win);
    wrefresh(username_win);
}

Auth_interface::~Auth_interface()
{
    delwin(username_win);
    delwin(password_win);
    delwin(info_win);
    endwin();
}

bool Auth_interface::web_request()
{
    std::string password = curl_easy_escape(&curl, password_text.c_str(), 0);

    curl.get(AUTH_ADDR(username_text, password));

    rapidjson::Document d;
    d.Parse(curl.response()->c_str());

    if (d.HasMember("error"))
    {
        error_text.clear();
        error_text = "Error: ";
        error_text += d["error"].GetString();
        return false;
    }

    id = d["user_id"].GetInt();
    token = d["access_token"].GetString();

    return true;
}


credentials Auth_interface::get()
{
    WINDOW* current_win = username_win;
    std::string* input_text = &username_text;
    bool return_flag = false;

    while(!return_flag)
    {
        werase(info_win);
        auto ch = getch();

        switch (ch)
        {
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case '\t': // KEY_TAB
                if (current_win == username_win)
                {
                    current_win = password_win;
                    input_text = &password_text;
                }
                else
                {
                    current_win = username_win;
                    input_text = &username_text;
                }
                break;
            case KEY_UP:
                current_win = username_win;
                input_text = &username_text;
                break;
            case KEY_DOWN:
                current_win = password_win;
                input_text = &password_text;
                break;
            case 127: // KEY_BACKSPACE
                input_text->pop_back();
                break;
            case '\n': // KEY_ENTER
                if (web_request())
                    return_flag = true;
                else
                    wprintw(info_win, error_text.c_str());
                break;
            default:
                *input_text += ch;
        }

        wrefresh(info_win);
        werase(current_win);

        if (input_text != &password_text)
            wprintw(current_win, input_text->c_str());
        else
            for (int i = 0; i < input_text->length(); ++i)
                wprintw(current_win, "*");

        wrefresh(current_win);

    }

    return {id, token};

}
