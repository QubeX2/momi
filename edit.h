/**
 *
 */
#pragma once
#ifndef __EDIT_H__
#define __EDIT_H__

#include "config.h"
#include "row.h"

typedef struct edit_state_st {
    int edit_rows; // height of text display
    int edit_cols; // width of text display
    int screen_rows; // height of working screen
    int screen_cols; // width of working screen
    int cursor_x;
    int cursor_y;
    int num_rows;
    int row_offset;
    row_st *rows;
    wchar_t status_msg[80];
    time_t status_msg_time;
    config_st settings;
} edit_state_st;

extern edit_state_st M;

#endif /* __EDIT_H__ */