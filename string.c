/**
 * 
 */
#include "string.h"

/**
 * 
 */
wchar_t *string_tolower(wchar_t *string)
{
    for(wchar_t *p = string; *p; p++) {
        *p = towlower(*p);
    }
    return string;
}