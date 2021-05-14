/**
 * 
 */

#include "file.h"
#include "edit.h"
#include "term.h"
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>
#include "string.h"

/**
 *
 */
uint32_t file_get_extension_e(wchar_t *basename)
{
    wchar_t *ext = wcsrchr(basename, '.');
    if(!ext) {
        if(wcscmp(string_tolower(basename), L"makefile") == 0) {
            return kMakefile;
        } else {
            if(wcscmp(string_tolower(ext + 1), L"c") == 0) {
                return kC;
            }
        }
    }
    return kUnknown;
}

/**
 * 
 */
void file_open(char *filename)
{
    buffer_st *buffer = buffer_get_current();

    char *name = basename(filename);

    size_t size = strlen(name) + 1;   
    buffer->filename = (wchar_t*)malloc(size * sizeof(wchar_t));
    size = mbstowcs(buffer->filename, name, size);   
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