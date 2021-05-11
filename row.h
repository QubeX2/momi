/**
 * 
 */
#pragma once
#ifndef __ROW_H__
#define __ROW_H__

#include <stdint.h>
#include <wchar.h>
#include "types.h"
#include "buffer.h"

typedef struct row_st {
    size_t size;
    wchar_t *chars;        // chars rendered to screen
    wchar_t *special;       // save tabs etc
    wchar_t *highlight;     // highligh text
    buffer_st *buffer;
} row_st;

void row_add(buffer_st *buffer, uint32_t at, char *s, size_t len);
void row_syntax(row_st *row);
void row_insert_wchar(row_st *row, uint32_t at, wchar_t c);
void row_proccess(row_st *row, wchar_t *chars, size_t len);
void row_free(row_st *row);

#endif /* __ROW_H__ */