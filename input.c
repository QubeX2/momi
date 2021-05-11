/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "input.h"
#include "edit.h"
#include "term.h"
#include "output.h"
#include "buffer.h"
#include "types.h"

/**
 * 
 */
void in_move_cursor(int direction)
{
    buffer_st *buffer = buffer_get_current();
    row_st *row = buffer->cursor_y < buffer->num_rows ? &buffer->rows[buffer->cursor_y] : NULL;

    if(direction == ES.config.keymap.key_up) {
        if(buffer->cursor_y != 0) {
            buffer->cursor_y--;
        } else {
            if(buffer->row_offset > 0) {
                buffer->row_offset--;
            }
        }
    }

    if(direction == ES.config.keymap.key_down) {
        if(buffer->cursor_y != ES.edit_rows - 1) {
            buffer->cursor_y++;
        } else if(buffer->row_offset != (buffer->num_rows - buffer->cursor_y)) {
            buffer->row_offset++;
        }
    }

    if(direction == ES.config.keymap.key_left) {
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
                buffer->cursor_x = buffer->rows[buffer->cursor_y].size;
            } else {
                buffer->cursor_x = 0;
            }
        }
    }

    if(direction == ES.config.keymap.key_right) {
        if(row && buffer->cursor_x < row->size) {
            if(row->special[buffer->cursor_x] == '\t') {
                while(buffer->cursor_x < row->size && row->special[buffer->cursor_x] == '\t') {
                    buffer->cursor_x++;
                }
            } else {
                buffer->cursor_x++;
            }
        } else if(row && buffer->cursor_x == row->size) {
            buffer->cursor_y++;
            buffer->cursor_x = 0;
        }
    }
}

/**
 * 
 */
void in_process_keypress()
{
    timeout(500);
    int ch0 = getch();
 
    if(ch0 == ERR) {
        return;
    }

    bool is_handled = false;

    buffer_st *buffer = buffer_get_current();
    if(buffer == NULL) {
        term_die("buffer");
    };
    row_st *row = buffer->cursor_y < buffer->num_rows ? &buffer->rows[buffer->cursor_y] : NULL;
    keymap_st keymap = ES.config.keymap;

    if(ch0 == keymap.key_quit) {
        term_close();
        exit(0);
    }

    // arrow keys
    if(ch0 == keymap.key_up) {                
        in_move_cursor(keymap.key_up);
        is_handled = true;
    }
    if(ch0 == keymap.key_down) {                
        in_move_cursor(keymap.key_down);
        is_handled = true;
    }
    if(ch0 == keymap.key_left) {                
        in_move_cursor(keymap.key_left);
        is_handled = true;
    }
    if(ch0 == keymap.key_right) {                
        in_move_cursor(keymap.key_right);
        is_handled = true;
    }

    // page up - page down
    if(ch0 == keymap.key_pagedown || ch0 == keymap.key_pageup) {
        int times = ES.edit_rows;
        while(times--) {
            in_move_cursor(ch0 == keymap.key_pagedown ? keymap.key_down : keymap.key_up);
        }
        is_handled = true;
    }

    // home and end
    int ch1 = 0, ch2 = 0;
    if(ch0 == 27) {
        ch1 = getch();
        if(ch1 == keymap.key_end[1] || ch1 == keymap.key_home[1]) {
            ch2 = getch();
            if(ch2 == keymap.key_end[2]) {
                buffer->cursor_x = row->size;
            } else if(ch2 == keymap.key_home[2]) {
                buffer->cursor_x = 0;
            }
            is_handled = true;
        }
    }

    if((iswalnum(ch0) || iswspace(ch0) || iswpunct(ch0)) && is_handled == false) {
        row_insert_wchar(&buffer->rows[buffer->cursor_y], buffer->cursor_x, ch0);
        buffer->cursor_x++;
    }

    out_status_message(L"KEY COMBO: %d, %d, %d | 0%o, 0%o, 0%o | %d", ch0, ch1, ch2, ch0, ch1, ch2);

    // snap x to end of line
    row = (buffer->cursor_y < buffer->num_rows) ? &buffer->rows[buffer->cursor_y] : NULL;
    uint32_t eol = row ? row->size : 0;
    if(buffer->cursor_x > eol) {
        buffer->cursor_x = eol;
    }

}