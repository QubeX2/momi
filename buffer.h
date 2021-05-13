/**
 * 
 */

#pragma once
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdint.h>
#include <wchar.h>
#include "types.h"
#include "row.h"


typedef struct buffer_st {
    uint32_t index;
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t num_rows;
    uint32_t row_offset;
    uint32_t current_line;    
    uint32_t dirty;
    row_st *rows;
    wchar_t status_msg[80];
    time_t status_msg_time;
    wchar_t *filename;
} buffer_st;

void buffer_insert(uint32_t at);
buffer_st *buffer_get_current();
buffer_st *buffer_get_at(uint32_t at);

#endif /* __BUFFER_H__ */