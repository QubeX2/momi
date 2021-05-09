/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <stdint.h>
#include "output.h"
#include "edit.h"

void out_rewdraw_screen()
{
    for(uint32_t y = 0; y < ES.edit_rows; y++) {
        uint32_t row = y + ES.row_offset;
        if(row >= ES.num_rows) {
            mvaddwstr(y,0,L"~");
        } else {
            mvaddwstr(y,0,ES.rows[y].chars);
        }
    }

    out_status_bar(L" File: %ls --- X:%d Y:%d W:%d H:%d", ES.filename == NULL ? L"[No name]" : ES.filename, ES.cursor_x, ES.cursor_y, ES.screen_cols, ES.screen_rows);
    move(ES.cursor_y, ES.cursor_x);
    out_refresh();
}

/**
 * 
 */
void out_status_bar(const wchar_t *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    wchar_t msg[ES.screen_cols + 1];
    wmemset(msg, '\0', ES.screen_cols);
    vswprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    for(uint32_t i=0;i<ES.screen_cols;i++) {
        if(msg[i] == '\0') {
            msg[i] = ' ';
        }
    }
    msg[ES.screen_cols] = '\0';
    attron(COLOR_PAIR(1));
    mvaddwstr(ES.screen_rows - 2, 0, msg);
    attroff(COLOR_PAIR(1));

    if(time(NULL) - ES.status_msg_time < 5) {
        mvaddwstr(ES.screen_rows - 1, 0, ES.status_msg);
    }

}

/**
 * 
 */
void out_status_message(const wchar_t *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    size_t size = sizeof(ES.status_msg) / sizeof(wchar_t);
    wmemset(ES.status_msg, '\0', size);
    vswprintf(ES.status_msg, size, fmt, ap);
    va_end(ap);

    for(int i=0; i< (int) size; i++) {
        if(ES.status_msg[i] == '\0') {
            ES.status_msg[i] = ' ';
        }
    }
    ES.status_msg[size] = '\0';
    ES.status_msg_time = time(NULL);
}

/**
 * 
 */
void out_refresh() {
    //refresh();
}