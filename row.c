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
void row_insert_wchar(row_st *row, uint32_t at, wchar_t c)
{
    if(at < 0 || at >= row->size) {
        return;
    }

    row->chars = realloc(row->chars, (row->size + 2) * sizeof(wchar_t));
    wmemmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->size++;
    row->chars[at] = c;

    row->buffer->dirty++;

}

/**
 * 
 */
void row_add(buffer_st *buffer, uint32_t at, char *s, size_t len)
{
    if(at < 0L || at > buffer->num_rows) {
        return;
    }

    // insert row in buffer->rows
    buffer->rows = realloc(buffer->rows, sizeof(row_st) * (buffer->num_rows + 1));
    memmove(&buffer->rows[at + 1], &buffer->rows[at], sizeof(row_st) * (buffer->num_rows - at));

    wchar_t *chars = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    size_t size = mbstowcs(chars, s, len);   
    if(size == (size_t)-1) {
        term_die("row_insert");
    }

    row_proccess(&buffer->rows[at], chars, size);

    buffer->rows[at].buffer = buffer;
    buffer->num_rows++;
    buffer->dirty++;
}

/**
 * 
 */
void row_proccess(row_st *row, wchar_t *chars, size_t len)
{
    uint16_t tabs = 0;
    for(uint16_t i = 0; i < len; i++) {
        if(chars[i] == '\t'){
            tabs++;
        }
    }
    size_t size = (len + (tabs * (ES.config.spaces_as_tab_count - 1)) + 1) * sizeof(wchar_t);

    row->chars = (wchar_t *)malloc(size);
    row->special = (wchar_t *)malloc(size);
    row->highlight = (wchar_t *)malloc(size);

    size_t idx = 0;
    for(size_t i = 0; i < len; i++) {
        if(chars[i] == '\t') {
            row->special[idx] = '\t';
            row->chars[idx++] = ES.config.space_wchar;
            for(uint8_t j = 0;j < ES.config.spaces_as_tab_count - 1; j++) {
                row->special[idx] = '\t';
                row->chars[idx++] = ES.config.space_wchar;
            }
        } else {
            row->special[idx] = '\0';
            row->chars[idx++] = chars[i];
        }
    }
    row->chars[idx] = '\0';
    row->special[idx] = '\0';
    row->size = idx;

    row_syntax(row);
}

/**
 * 
 */
void row_syntax(row_st *row)
{
    row->highlight = realloc(row->highlight, row->size * sizeof(wchar_t));
    wmemset(row->highlight, 0, row->size);    
}

/**
 * 
 */
void row_free(row_st *row)
{
    free(row->chars);
    free(row->highlight);
    free(row->special);
}