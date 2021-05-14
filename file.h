/**
 *
 */
/**
 * 
 */
#pragma once
#ifndef __FILE_H__
#define __FILE_H__

#include <stdint.h>
#include <wchar.h>

void file_open(char* filename);
uint32_t file_get_extension_e(wchar_t *basename);

#endif /* __FILE_H__ */