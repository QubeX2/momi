/**
 * 
 */
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "edit.h"

/**
 *
 */
void buffer_insert(uint32_t at)
{
    ES.buffers = realloc(ES.buffers, sizeof(buffer_st) * (ES.num_buffers + 1));
    memmove(&ES.buffers[at + 1], &ES.buffers[at], sizeof(buffer_st) * (ES.num_buffers - at));
    ES.num_buffers++;
    ES.cur_buf = at;
    ES.buffers[ES.cur_buf].cursor_x = 0;
    ES.buffers[ES.cur_buf].cursor_y = 0;
    ES.buffers[ES.cur_buf].row_offset = 0;
    ES.buffers[ES.cur_buf].current_line = 0;
    ES.buffers[ES.cur_buf].num_rows = 0;
    ES.buffers[ES.cur_buf].dirty = 0;
    ES.buffers[ES.cur_buf].rows = NULL;
    ES.buffers[ES.cur_buf].filename = NULL;
}

/**
 * 
 */
buffer_st *buffer_get_current()
{
    return (ES.cur_buf >= 0 && ES.cur_buf < ES.num_buffers) ? &ES.buffers[ES.cur_buf] : NULL;
}

/**
 * 
 */
buffer_st *buffer_get_at(uint32_t at)
{
    return (at >= 0 && at < ES.num_buffers) ? &ES.buffers[at] : NULL;
}
