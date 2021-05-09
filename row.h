/**
 * 
 */
#pragma once
#ifndef __ROW_H__
#define __ROW_H__

#include <wchar.h>

typedef struct row_st {
    size_t char_size;
    wchar_t *chars;         // storing original chars
    size_t render_size;
    wchar_t *render;        // chars rendered to screen
    wchar_t *highlight;     // highligh data
} row_st;

#endif /* __ROW_H__ */