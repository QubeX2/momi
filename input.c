/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <stdlib.h>
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

    switch(ch) {
        case CTRL_KEY('q'):
            term_close();
            exit(0);
            break;

        case 'q':
            break;

        case KEY_UP:
            if(M.cursor_y > 0) {
                M.cursor_y--;
            }
            break;

        case KEY_DOWN:
            if(M.cursor_y < M.edit_rows) {
                M.cursor_y++;
            }
            break;

        case KEY_LEFT:
            if(M.cursor_x > 0) {
                M.cursor_x--;
            }
            break;

        case KEY_RIGHT:
            if(M.cursor_x < M.edit_cols) {
                M.cursor_x++;
            }
            break;
        
    }
}