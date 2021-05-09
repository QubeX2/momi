/**
 *
 */
#pragma once
#ifndef __EDIT_H__
#define __EDIT_H__

#include <stdint.h>
#include "config.h"
#include "row.h"

typedef struct edit_state_st {
    uint32_t edit_rows; // height of text display
    uint32_t edit_cols; // width of text display
    uint32_t screen_rows; // height of working screen
    uint32_t screen_cols; // width of working screen
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t num_rows;
    uint64_t row_offset;
    uint32_t dirty;
    row_st *rows;
    wchar_t status_msg[80];
    time_t status_msg_time;
    wchar_t *filename;
    config_st config;
} edit_state_st;

extern edit_state_st ES;

#endif /* __EDIT_H__ */