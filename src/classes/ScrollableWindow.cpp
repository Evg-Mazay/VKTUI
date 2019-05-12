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
    else
    {
        int offset = ((depth + depth_offset) + lines) % real_lines;

        prefresh(pad, (depth + depth_offset) % real_lines, 0, y, x, y + lines-offset-1, x + cols);

        prefresh(pad, 0, 0, y + lines-offset, x, y + lines-1, x + cols);

        
    }
}

WINDOW* ScrollableWindow::win()
{
    return pad;
}

void ScrollableWindow::print(std::wstring str)
{
    int y, x;
    int old_y;
    getyx(pad, old_y, x);

    for (int i = 0; i < str.length(); ++i)
    {
        cchar_t c = {A_NORMAL, {str[i]}};

        getyx(pad, y, x);
        if ((str[i] == L'\n' &&  y == real_lines-1) ||
             (y == real_lines-1 && x == cols-1))
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
    int old_depth = depth;
    depth += y - old_y;

    // if (depth - old_depth) frontend->print_debug_message(L"depth: " + to_wstring(depth));
}

void ScrollableWindow::scrll(int n)
{
    if (depth_offset + n >= 0)
        depth_offset = 0;
    else if (depth_offset + n <= -real_lines + lines)
        depth_offset = -real_lines + lines;
    else
        depth_offset += n;

    // frontend->print_debug_message(L"depth: " + to_wstring(depth_offset));
}


int ScrollableWindow::edit(std::wstring str, int depth)
{
    int old_y, old_x;
    getyx(pad, old_y, old_x);

    if (depth > 0)
        wmove(pad, (depth + depth_offset) % real_lines, 0);
    else
        wmove(pad, depth+lines, 0);

    int y, x;
    getyx(pad, y, x);
    // frontend->print_debug_message(L"moved: " + to_wstring(y) + L" " + to_wstring(x));

    waddwstr(pad, str.c_str());

    refresh();

    wmove(pad, old_y, old_x);
}




