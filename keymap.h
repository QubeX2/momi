/**
 * 
 */
#pragma once
#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include <wchar.h>

typedef struct keymap_st {
    wint_t key_left;
    wint_t key_right;
    wint_t key_up;
    wint_t key_down;
    wint_t key_pagedown;
    wint_t key_pageup;
    wint_t key_quit;
    wint_t key_save;
    wint_t key_copy;
    wint_t key_paste;
    wint_t key_delete;
    wint_t key_backspace;
    wint_t key_end[3];
    wint_t key_home[3];
} keymap_st;

#endif /* __KEYMAP_H__ */