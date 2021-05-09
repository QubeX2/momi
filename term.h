/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>


void term_die(const char *s);
void term_init();
void term_close();
