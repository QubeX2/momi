/**
 * MOMI Editor by QubeX2
 * email: qubex2@gmail.com
 */
#include "term.h"
#include "input.h"
#include "output.h"
#include "edit.h"

#define MOMI_VERSION "0.0.1"

int main() 
{
    term_init();

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