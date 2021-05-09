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

/**
 * 
 */
void row_insert(uint64_t at, char *s, size_t len)
{
    if(at < 0L || at > ES.num_rows) {
        return;
    }

    ES.rows = realloc(ES.rows, sizeof(row_st) * (ES.num_rows + 1));
    memmove(&ES.rows[at + 1], &ES.rows[at], sizeof(row_st) * (ES.num_rows - at));

    char *tmp = (char*)malloc(len + 1);
    memcpy(tmp, s, len);
    tmp[len] = '\0';

    wchar_t* buf = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    size_t size = mbstowcs(buf, tmp, len);   
    free(tmp);
    if(size == (size_t)-1) {
        term_die("row_insert");
    }

    ES.rows[at].csize = size;
    ES.rows[at].chars = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    wmemcpy(ES.rows[at].chars, buf, size);
    ES.rows[at].chars[len] = '\0';
    ES.rows[at].rsize = 0;
    ES.rows[at].render = NULL;
    ES.rows[at].highlight = NULL;

    free(buf);

    ES.num_rows++;
    ES.dirty++;
}