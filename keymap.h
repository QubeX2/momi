/**
 * 
 */
#pragma once
#ifndef __KEYMAP_H__
#define __KEYMAP_H__

typedef struct keymap_st {
    int key_left;
    int key_right;
    int key_up;
    int key_down;
    int key_pagedown;
    int key_pageup;
    int key_quit;
    int key_save;
    int key_copy;
    int key_paste;
    int key_delete;
    int key_backspace;
    int key_end[3];
    int key_home[3];
} keymap_st;

#endif /* __KEYMAP_H__ */