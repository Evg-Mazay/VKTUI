#ifndef __SCROLLABLE_WINDOW__
#define __SCROLLABLE_WINDOW__

#include <curses.h>
#include <string>

/*

Ncurses "window" that can be scrolled backward
Consists of big pad and sliding window

    ╔════════════════════╗   ╔════════════════════╗   ╔════════════════════╗
0   ║ hi                 ║   ║│seven             │║   ║ seven              ║
1   ║ hi                 ║   ║│eight             │║   ║ eight              ║
2   ║ hi                 ║   ║└────────↓─────────┘║   ║ hi                 ║
3   ║┌──────────────────┐║   ║ hi                 ║   ║ hi                 ║
4   ║│hi                │║   ║ hi                 ║   ║ hi                 ║
5   ║│hello             │║   ║ hello              ║   ║┌────────↑─────────┐║
6   ║│world!            │║   ║ world!             ║   ║│world             │║
7   ║│one               │║   ║ one                ║   ║│one               │║
8   ║│two               │║   ║┌──────────────────┐║   ║│two               │║
9   ║│three             │║   ║│three             │║   ║│three             │║
10  ║└────────↓─────────┘║   ║│four              │║   ║│four              │║
11  ║                    ║   ║│five              │║   ║│five              │║
12  ║                    ║   ║│six               │║   ║└──────────────────┘║
    ╚════════════════════╝   ╚════════════════════╝   ╚════════════════════╝

    printed: "hi↲" 5 times   printed: "hello↲world    scrolled 3 lines up 
                             ↲one↲two↲ ... ↲eight↲"

    depth: 4                 depth: 9                 depth: 9
    depth_offset: 0          depth_offset: 0          depth_offset: -3

*/

class ScrollableWindow
{
    int y, x;           // window position on screen
    int cols, lines;    // window size (height, width)
    int real_lines;     // pad size (height)

    long depth;             // how deep are we from pad start, may be > than real_lines
    int depth_offset = 0;   // user can scroll it from 0 (last printed line) to -real_lines + lines

    WINDOW* pad;

public:
    ScrollableWindow(int _real_lines, int _lines, int _cols, int _y, int _x);

    // print a string on pad and move the window
    long print(std::wstring str);

    // print a string at specified depth
    int edit(std::wstring str, long _depth);

    // set ncurses attribute on pad
    void attr(unsigned long attr);

    // draw window
    void refresh();

    // erase pad
    void erase();

    // delete window, create it again and clear
    void reset();

    // delete window
    void del();

    // scroll window up (n < 0) or down (n > 0)
    void scrll(int n);
};


#endif