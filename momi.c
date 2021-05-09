/**
 * MOMI Editor by QubeX2
 * email: qubex2@gmail.com
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include "term.h"
#include "input.h"
#include "output.h"
#include "edit.h"
#include "file.h"

#define MOMI_VERSION "0.0.1"

int main(int argc, char *argv[]) 
{
    term_init();

    if(argc >= 2) {
        file_open(argv[1]);
    }

    while(1) {
        out_rewdraw_screen();
        in_process_keypress();
        if(is_term_resized(ES.screen_rows, ES.screen_cols)) {
            getmaxyx(stdscr, ES.screen_rows, ES.screen_cols);
        }
    }

    term_close();
    return 0;
}