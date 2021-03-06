/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <stdint.h>
#include "output.h"
#include "edit.h"
#include "buffer.h"

/**
 * 
 */
void out_rewdraw_screen()
{
    buffer_st *buffer = buffer_get_current();

    for (uint32_t y = 0; y < ES.edit_rows; y++)
    {
        uint32_t line = y + buffer->row_offset;
        if (line >= buffer->num_rows)
        {
            mvaddwstr(y, 0, L"~");
        }
        else
        {
            mvaddwstr(y, 0, buffer->rows[line].chars);
        }
        clrtoeol();
    }

    out_status_bar(L" File: %ls%s --- X:%d Y:%d W:%d H:%d O:%d C:%d",
                        buffer->filename == NULL ? L"[No name]" : buffer->filename,
                        (buffer->dirty ? "*" : ""),
                        buffer->cursor_x,
                        buffer->cursor_y,
                        ES.screen_cols,
                        ES.screen_rows,
                        buffer->row_offset,
                        buffer->current_line
                   );
    move(buffer->cursor_y, buffer->cursor_x);
    out_refresh();
}

/**
 * 
 */
void out_status_bar(const wchar_t *fmt, ...)
{
    buffer_st *buffer = buffer_get_current();

    va_list ap;
    va_start(ap, fmt);
    wchar_t msg[ES.screen_cols + 1];
    wmemset(msg, '\0', ES.screen_cols);
    vswprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    for (uint32_t i = 0; i < ES.screen_cols; i++)
    {
        if (msg[i] == '\0')
        {
            msg[i] = ' ';
        }
    }
    msg[ES.screen_cols] = '\0';
    attron(COLOR_PAIR(1));
    mvaddwstr(ES.screen_rows - 2, 0, msg);
    attroff(COLOR_PAIR(1));

    if (time(NULL) - buffer->status_msg_time < 5)
    {
        mvaddwstr(ES.screen_rows - 1, 0, buffer->status_msg);
    }
}

/**
 * 
 */
void out_status_message(const wchar_t *fmt, ...)
{
    buffer_st *buffer = buffer_get_current();

    va_list ap;
    va_start(ap, fmt);
    size_t size = sizeof(buffer->status_msg) / sizeof(wchar_t);
    wmemset(buffer->status_msg, '\0', size);
    vswprintf(buffer->status_msg, size, fmt, ap);
    va_end(ap);

    for (int i = 0; i < (int)size; i++)
    {
        if (buffer->status_msg[i] == '\0')
        {
            buffer->status_msg[i] = ' ';
        }
    }
    buffer->status_msg[size] = '\0';
    buffer->status_msg_time = time(NULL);
}

/**
 * 
 */
void out_refresh()
{
    //refresh();
}