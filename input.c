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
void in_process_keypress()
{
    timeout(500);
    int ch = getch();

    const char *name = key_name(ch);
    out_status_message(L"KEY: %s, %c, %d", name, ch, ch);

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
            if(buffer->cursor_y > 0) {
                buffer->cursor_y--;
            }
            break;

        case KEY_DOWN:
            if(buffer->cursor_y < buffer->num_rows) {
                buffer->cursor_y++;
            }
            break;

        case KEY_LEFT:
            if(buffer->cursor_x != 0) {
                buffer->cursor_x--;
            } else if (buffer->cursor_y > 0) {
                buffer->cursor_y--;
                if(buffer->cursor_y < buffer->num_rows) {
                    buffer->cursor_x = buffer->rows[buffer->cursor_y].csize;
                } else {
                    buffer->cursor_x = 0;
                }
            }
            break;

        case KEY_RIGHT:
            if(row && buffer->cursor_x < row->csize) {
                buffer->cursor_x++;
            } else if(row && buffer->cursor_x == row->csize) {
                buffer->cursor_y++;
                buffer->cursor_x = 0;
            }
            break;

    }

    // snap x to end of line
    row = (buffer->cursor_y < buffer->num_rows) ? &buffer->rows[buffer->cursor_y] : NULL;
    uint32_t eol = row ? row->csize : 0;
    if(buffer->cursor_x > eol) {
        buffer->cursor_x = eol;
    }

}