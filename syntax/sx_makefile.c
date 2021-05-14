/**
 * 
 */
#include <stdio.h>
#include "sx_makefile.h"

wchar_t *syntax(wchar_t *chars, size_t len) {
    printf("%ls %zu", chars, len);
    return L"Mikael Andersson";
}
