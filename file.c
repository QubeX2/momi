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
    size_t size = strlen(filename) + 1;   
    ES.filename = (wchar_t*)malloc(size * sizeof(wchar_t));
    size = mbstowcs(ES.filename, filename, size);   
    if(size == (size_t)-1) {
        term_die("open file");
    }

    FILE *fp = fopen(filename, "r");
    if(!fp) term_die("fopen");

    char *line = NULL;
    size_t cap = 0;
    ssize_t len = 0;

    while((len = getline(&line, &cap, fp)) != -1) {
        while(len > 0 && (line[len -1] == '\n' || line[len - 1] == '\r')) {
            len--;
        }
        row_insert(ES.num_rows, line, len);
    }
        
    free(line);
    fclose(fp);
    ES.dirty = 0;    
}