/**
 * 
 */

#include "file.h"
#include "edit.h"
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
    M.filename = malloc(size * sizeof(wchar_t));
    size = mbstowcs(M.filename, filename, size);    
}