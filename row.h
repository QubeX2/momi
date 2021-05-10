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
    size_t csize;
    wchar_t *chars;         // original chars
    size_t rsize;
    wchar_t *render;        // chars rendered to screen
    wchar_t *special;       // save tabs etc
    wchar_t *highlight;     // highligh text
} row_st;

void row_insert(buffer_st *buffer, uint32_t at, char *s, size_t len);
void row_proccess(row_st *row);
void row_syntax(row_st *row);
void row_free(row_st *row);

#endif /* __ROW_H__ */