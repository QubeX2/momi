/**
 * 
 */

#include <stdint.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "row.h"
#include "edit.h"
#include "term.h"
#include "buffer.h"

/**
 * 
 */
void row_insert(buffer_st *buffer, uint32_t row_at, char *s, size_t len)
{

    if(row_at < 0L || row_at > buffer->num_rows) {
        return;
    }

    buffer->rows = realloc(buffer->rows, sizeof(row_st) * (buffer->num_rows + 1));
    memmove(&buffer->rows[row_at + 1], &buffer->rows[row_at], sizeof(row_st) * (buffer->num_rows - row_at));

    buffer->rows[row_at].chars = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    size_t size = mbstowcs(buffer->rows[row_at].chars, s, len);   
    if(size == (size_t)-1) {
        term_die("row_insert");
    }
    buffer->rows[row_at].chars[len] = '\0';
    buffer->rows[row_at].csize = size;
    buffer->rows[row_at].rsize = 0;
    buffer->rows[row_at].render = NULL;
    buffer->rows[row_at].highlight = NULL;
    buffer->num_rows++;
    buffer->dirty++;
}