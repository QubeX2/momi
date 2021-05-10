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
    wchar_t *chars;         // storing original chars
    size_t rsize;
    wchar_t *render;        // chars rendered to screen
    wchar_t *highlight;     // highligh data
} row_st;

void row_insert(buffer_st *buffer, uint32_t row_at, char *s, size_t len);

#endif /* __ROW_H__ */