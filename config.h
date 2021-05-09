/**
 * 
 */
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct config_st {
    bool use_spaces_as_tabs;
    uint8_t tab_spaces;
} config_st;

#endif /* __CONFIG_H__ */