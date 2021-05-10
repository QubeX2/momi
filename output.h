/**
 * 
 */
#pragma once
#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <wchar.h>

void out_status_bar(const wchar_t *fmt, ...);
void out_status_message(const wchar_t *fmt, ...);
void out_rewdraw_screen();
void out_refresh();
void out_scroll();

#endif /* __OUTPUT_H__ */