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
void row_insert(buffer_st *buffer, uint32_t at, char *s, size_t len)
{

    if(at < 0L || at > buffer->num_rows) {
        return;
    }

    buffer->rows = realloc(buffer->rows, sizeof(row_st) * (buffer->num_rows + 1));
    memmove(&buffer->rows[at + 1], &buffer->rows[at], sizeof(row_st) * (buffer->num_rows - at));

    buffer->rows[at].chars = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    size_t size = mbstowcs(buffer->rows[at].chars, s, len);   
    if(size == (size_t)-1) {
        term_die("row_insert");
    }

    buffer->rows[at].chars[len] = '\0';
    buffer->rows[at].csize = size;
    buffer->rows[at].rsize = 0;
    buffer->rows[at].render = NULL;
    buffer->rows[at].highlight = NULL;
    buffer->rows[at].special = NULL;    
    buffer->num_rows++;
    buffer->dirty++;

    row_proccess(&buffer->rows[at]);
}

/**
 * 
 */
void row_proccess(row_st *row)
{
    uint16_t tabs = 0;
    for(uint16_t i = 0; i < row->csize; i++) {
        if(row->chars[i] == '\t'){
            tabs++;
        }
    }
    size_t size = (row->csize + (tabs * (ES.config.spaces_as_tab_count - 1)) + 1) * sizeof(wchar_t);
    free(row->render);
    //free(row->special);
    free(row->highlight);
    row->render = (wchar_t *)malloc(size);
    row->special = (wchar_t *)malloc(size);
    row->highlight = (wchar_t *)malloc(size);
    

    uint32_t idx = 0;
    for(uint32_t i = 0; i < row->csize; i++) {
        if(row->chars[i] == '\t') {
            row->special[idx] = '\t';
            row->render[idx++] = ES.config.space_wchar;
            for(uint8_t j = 0;j < ES.config.spaces_as_tab_count - 1; j++) {
                row->special[idx] = '\t';
                row->render[idx++] = ES.config.space_wchar;
            }
        } else {
            row->special[idx] = '\0';
            row->render[idx++] = row->chars[i];
        }
    }
    row->render[idx] = '\0';
    row->special[idx] = '\0';
    row->rsize = idx;

    wmemset(row->highlight, '\0', idx);


    row_syntax(row);
}

/**
 * 
 */
void row_syntax(row_st *row)
{
    row->highlight = realloc(row->highlight, row->rsize * sizeof(wchar_t));
    wmemset(row->highlight, 0, row->rsize);    
}

/**
 * 
 */
void row_free(row_st *row)
{
    free(row->render);
    free(row->chars);
    free(row->highlight);
    free(row->special);
}