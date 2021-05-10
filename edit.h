/**
 *
 */
#pragma once
#ifndef __EDIT_H__
#define __EDIT_H__

#include <stdint.h>
#include "config.h"
#include "buffer.h"

typedef struct edit_state_st {
    uint32_t edit_rows; // height of text display
    uint32_t edit_cols; // width of text display
    uint32_t screen_rows; // height of working screen
    uint32_t screen_cols; // width of working screen
    uint32_t num_buffers;
    uint32_t cur_buf;
    buffer_st *buffers;
    config_st config;
} edit_state_st;

extern edit_state_st ES;

#endif /* __EDIT_H__ */