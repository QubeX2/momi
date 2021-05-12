/**
 * 
 */

#include "file.h"
#include "edit.h"
#include "term.h"
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * 
 */
void file_open(char *filename)
{
    buffer_st *buffer = buffer_get_current();

    size_t size = strlen(filename) + 1;   
    buffer->filename = (wchar_t*)malloc(size * sizeof(wchar_t));
    size = mbstowcs(buffer->filename, filename, size);   
    if(size == (size_t)-1) {
        term_die("open file");
    }

    FILE *fp = fopen(filename, "r");
    if(!fp) term_die("fopen");

    char *line = NULL;
    size_t cap = 0;
    ssize_t len = 0;

    while((len = getline(&line, &cap, fp)) != -1) {
        while(len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            len--;
        }
        line[len] = '\0';
        row_add_mbs(buffer, buffer->num_rows, line, len);
    }
        
    free(line);
    fclose(fp);
    buffer->dirty = 0;    
}