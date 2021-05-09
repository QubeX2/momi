/**
 *
 */

#include "term.h"
#include "edit.h"

void term_die(const char *msg)
{
    perror(msg);
    term_close();
    exit(1);
}

void term_init()
{
    setlocale(LC_ALL, "");
    initscr(); // start ncurses
    raw();
    getmaxyx(stdscr, M.screen_rows, M.screen_cols);    
    noecho();  // don't echo keystrokes
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE); // enable special keys    

    if(has_colors() == TRUE) {
        start_color();
    }

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    M.edit_rows = M.screen_rows - 2;
    M.edit_cols = M.screen_cols;
    M.cursor_x = 0;
    M.cursor_y = 0;
    M.row_offset = 0;
    M.num_rows = 0;
}

void term_close()
{
    endwin();
}