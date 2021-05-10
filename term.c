/**
 *
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "term.h"
#include "edit.h"

void term_die(const char *msg)
{
    term_close();
    perror(msg);
    exit(1);
}

void term_init()
{
    setlocale(LC_ALL, "");
    initscr(); // start ncurses
    raw();
    getmaxyx(stdscr, ES.screen_rows, ES.screen_cols);
    noecho(); // don't echo keystrokes
    nonl();
    intrflush(stdscr, false);
    keypad(stdscr, TRUE); // enable special keys

    if (has_colors() == true) {
        start_color();
    }

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    ES.edit_rows = ES.screen_rows - 2;
    ES.edit_cols = ES.screen_cols;

    // create empty buffer
    buffer_insert(0);

    ES.config.use_spaces_as_tabs = true;
    ES.config.spaces_as_tab_count = 8;
    ES.config.space_wchar = ' ';
}

void term_close()
{
    endwin();
}