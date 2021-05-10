/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>
#include "input.h"
#include "edit.h"
#include "term.h"
#include "output.h"
#include "buffer.h"

#define CTRL_KEY(k) ((k) & 0x1f)

/**
 * 
 */
void in_move_cursor(int direction)
{
    buffer_st *buffer = buffer_get_current();
    row_st *row = buffer->cursor_y < buffer->num_rows ? &buffer->rows[buffer->cursor_y] : NULL;

    switch(direction) {
        case KEY_UP:
            if(buffer->cursor_y != 0) {
                buffer->cursor_y--;
            } else {
                if(buffer->row_offset > 0) {
                    buffer->row_offset--;
                }
            }
            break;

        case KEY_DOWN:
            if(buffer->cursor_y != ES.edit_rows - 1) {
                buffer->cursor_y++;
            } else if(buffer->row_offset != (buffer->num_rows - buffer->cursor_y)) {
                buffer->row_offset++;
            }
            break;

        case KEY_LEFT:
            if(buffer->cursor_x != 0) {
                buffer->cursor_x--;
                if(row->special[buffer->cursor_x] == '\t') {
                    while(buffer->cursor_x > 0 && row->special[buffer->cursor_x] == '\t') {
                        buffer->cursor_x--;
                    }
                }
            } else if (buffer->cursor_y > 0) {
                buffer->cursor_y--;
                if(buffer->cursor_y < buffer->num_rows) {
                    buffer->cursor_x = buffer->rows[buffer->cursor_y].rsize;
                } else {
                    buffer->cursor_x = 0;
                }
            }
            break;

        case KEY_RIGHT:
            if(row && buffer->cursor_x < row->rsize) {
                if(row->special[buffer->cursor_x] == '\t') {
                    while(buffer->cursor_x < row->rsize && row->special[buffer->cursor_x] == '\t') {
                        buffer->cursor_x++;
                    }
                } else {
                    buffer->cursor_x++;
                }
            } else if(row && buffer->cursor_x == row->rsize) {
                buffer->cursor_y++;
                buffer->cursor_x = 0;
            }
            break;

    }
}

/**
 * 
 */
void in_process_keypress()
{
    timeout(500);
    int ch = getch();

    const char *name = key_name(ch);
    out_status_message(L"KEY: %ls, %c, %ld", name, ch, ch);

    buffer_st *buffer = buffer_get_current();
    if(buffer == NULL) {
        term_die("buffer");
    };

    row_st *row = buffer->cursor_y < buffer->num_rows ? &buffer->rows[buffer->cursor_y] : NULL;

    switch(ch) {
        case CTRL_KEY('q'):
            term_close();
            exit(0);
            break;

        case KEY_UP:
            in_move_cursor(KEY_UP);
            break;

        case KEY_DOWN:
            in_move_cursor(KEY_DOWN);
            break;

        case KEY_LEFT:
            in_move_cursor(KEY_LEFT);
            break;

        case KEY_RIGHT:
            in_move_cursor(KEY_RIGHT);
            break;

        case KEY_NPAGE:
        case KEY_PPAGE:
            {
                int times = ES.edit_rows;
                while(times--) {
                    in_move_cursor(ch == KEY_NPAGE ? KEY_DOWN : KEY_UP);
                }
            }
            break;

        case KEY_HOME:
            buffer->cursor_x = 0;
            break;

        case KEY_END:
            buffer->cursor_x = row->rsize;
            break;

    }

    // snap x to end of line
    row = (buffer->cursor_y < buffer->num_rows) ? &buffer->rows[buffer->cursor_y] : NULL;
    uint32_t eol = row ? row->csize : 0;
    if(buffer->cursor_x > eol) {
        buffer->cursor_x = eol;
    }

}