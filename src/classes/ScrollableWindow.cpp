#include <curses.h>

#include "ScrollableWindow.h"
#include "../Frontend.h"

extern Frontend* frontend;

ScrollableWindow::ScrollableWindow(int _real_lines, int _lines, int _cols, int _y, int _x)
{
    y = _y;
    x = _x;
    cols = _cols;
    lines = _lines;
    real_lines = _real_lines;

    depth = -lines;

    pad = newpad(real_lines, cols);
    leaveok(pad, TRUE);

    wmove(pad, 0, 0);
}

void ScrollableWindow::erase()
{
    werase(pad);
}

void ScrollableWindow::refresh()
{
    if ((depth + depth_offset) % real_lines + lines < real_lines)
    {
        prefresh(pad, (depth + depth_offset) % real_lines, 0, y, x, y + lines-1, x + cols);
    }
    else // here we actually draw two windows
    {
        int offset = ((depth + depth_offset) + lines) % real_lines;

        prefresh(pad, (depth + depth_offset) % real_lines, 0, y, x, y + lines-offset-1, x + cols);

        prefresh(pad, 0, 0, y + lines-offset, x, y + lines-1, x + cols);

        
    }
}

long ScrollableWindow::print(std::wstring str)
{
    /*
    !!!! Наверное, потребуется очищать строку, в которой будет печать
            (Но это не точно)
    */

    int y, x;
    int old_y;
    getyx(pad, old_y, x);

    for (unsigned i = 0; i < str.length(); ++i)
    {
        cchar_t c = {A_NORMAL, {str[i]}};

        getyx(pad, y, x);
        if ((str[i] == L'\n' &&  y == real_lines-1) ||
             (y == real_lines-1 && x == cols))
        {
            wadd_wch(pad, &c);
            old_y = y - old_y-1;
            wmove(pad, 0,0);
        }
        else
        {
            wadd_wch(pad, &c);
        }
    }

    getyx(pad, y, x);
    long old_depth = depth;
    depth += y - old_y;

    return old_depth;

    // if (depth - old_depth) frontend->print_debug_message(L"depth: " + to_wstring(depth));
}

void ScrollableWindow::scrll(int n)
{
    if (depth_offset + n >= 0)
        depth_offset = 0;
    else if (depth + depth_offset + n < -1)
        return;
    else if (depth_offset + n <= -real_lines + lines)
        depth_offset = -real_lines + lines;
    else
        depth_offset += n;

    // frontend->print_debug_message(L"depth: " + to_wstring(depth) + L" " + to_wstring(depth_offset));
}


int ScrollableWindow::edit(std::wstring str, long _depth)
{
    // if (_depth <= )
    //     return 1;

    int old_y, old_x;
    getyx(pad, old_y, old_x);

    if (_depth > 0)
        wmove(pad, (_depth + depth_offset) % real_lines, 0);
    else
        wmove(pad, _depth+lines, 0);

    int y, x;
    getyx(pad, y, x);
	y = y+1;
	y = y-1;
    // frontend->print_debug_message(L"moved: " + to_wstring(y) + L" " + to_wstring(x));

    waddwstr(pad, str.c_str());

    refresh();

    wmove(pad, old_y, old_x);

    return 0;
}


void ScrollableWindow::reset()
{
    delwin(pad);

    depth = -lines;
    pad = newpad(real_lines, cols);
    wmove(pad, 0, 0);
}

void ScrollableWindow::del()
{
    delwin(pad);
}

void ScrollableWindow::attr(unsigned long attr)
{
    wattrset(pad, attr);
}













