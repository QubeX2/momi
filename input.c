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

    row_st *row = ES.cursor_y < ES.num_rows ? &ES.rows[ES.cursor_y] : NULL;

    switch(ch) {
        case CTRL_KEY('q'):
            term_close();
            exit(0);
            break;

        case KEY_UP:
            if(ES.cursor_y > 0) {
                ES.cursor_y--;
            }
            break;

        case KEY_DOWN:
            if(ES.cursor_y < ES.num_rows) {
                ES.cursor_y++;
            }
            break;

        case KEY_LEFT:
            if(ES.cursor_x != 0) {
                ES.cursor_x--;
            } else if (ES.cursor_y > 0) {
                ES.cursor_y--;
                if(ES.cursor_y < ES.num_rows) {
                    ES.cursor_x = ES.rows[ES.cursor_y].csize;
                } else {
                    ES.cursor_x = 0;
                }
            }
            break;

        case KEY_RIGHT:
            if(row && ES.cursor_x < row->csize) {
                ES.cursor_x++;
            } else if(row && ES.cursor_x == row->csize) {
                ES.cursor_y++;
                ES.cursor_x = 0;
            }
            break;

    }

    // snap x to end of line
    row = (ES.cursor_y < ES.num_rows) ? &ES.rows[ES.cursor_y] : NULL;
    uint32_t eol = row ? row->csize : 0;
    if(ES.cursor_x > eol) {
        ES.cursor_x = eol;
    }

}