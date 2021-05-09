/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include "output.h"
#include "edit.h"

void out_rewdraw_screen()
{
    for(int y = 0; y < M.edit_rows; y++) {
        int row = y + M.row_offset;
        if(row >= M.num_rows) {
            mvaddwstr(y,0,L"~");
        }
    }
    out_status_bar(L"X:%d Y:%d W:%d H:%d", M.cursor_x, M.cursor_y, M.screen_cols, M.screen_rows);
    move(M.cursor_y, M.cursor_x);
    out_refresh();
}

/**
 * 
 */
void out_status_bar(const wchar_t *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    wchar_t msg[M.screen_cols + 1];
    wmemset(msg, '\0', M.screen_cols);
    vswprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    for(int i=0;i<M.screen_cols;i++) {
        if(msg[i] == '\0') {
            msg[i] = ' ';
        }
    }
    msg[M.screen_cols] = '\0';
    attron(COLOR_PAIR(1));
    mvaddwstr(M.screen_rows - 2, 0, msg);
    attroff(COLOR_PAIR(1));

    if(time(NULL) - M.status_msg_time < 5) {
        mvaddwstr(M.screen_rows - 1, 0, M.status_msg);
    }

}

/**
 * 
 */
void out_status_message(const wchar_t *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    wmemset(M.status_msg, '\0', sizeof(M.status_msg));
    vswprintf(M.status_msg, sizeof(M.status_msg), fmt, ap);
    va_end(ap);

    for(int i=0; i< (int) (sizeof(M.status_msg) / sizeof(wchar_t)); i++) {
        if(M.status_msg[i] == '\0') {
            M.status_msg[i] = ' ';
        }
    }
    M.status_msg[sizeof(M.status_msg)/sizeof(wchar_t) - 1] = '\0';
    M.status_msg_time = time(NULL);
}

/**
 * 
 */
void out_refresh() {
    //refresh();
}