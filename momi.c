/**
 * MOMI Editor by QubeX2
 * email: qubex2@gmail.com
 */
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
        if(is_term_resized(M.screen_rows, M.screen_cols)) {
            getmaxyx(stdscr, M.screen_rows, M.screen_cols);
        }
    }

    term_close();
    return 0;
}