#ifndef __SCROLLABLE_WINDOW__
#define __SCROLLABLE_WINDOW__

#include <curses.h>
#include <string>

class ScrollableWindow
{
    int y;
    int x;
    int cols;
    int lines;
    int real_lines;

    int depth;
    int depth_offset = 0;

    WINDOW* pad;

public:
    ScrollableWindow(int _real_lines, int _lines, int _cols, int _y, int _x);
    
    void print(std::wstring str);
    int edit(std::wstring str, int depth);

    void refresh();
    void erase();
    WINDOW* win();

    void scrll(int n);
};


#endif