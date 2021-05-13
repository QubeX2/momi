/**
 * 
 */
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "input.h"
#include "edit.h"
#include "term.h"
#include "output.h"
#include "buffer.h"
#include "types.h"

/**
 * 
 */
static int _in_get_special_sequence(wint_t ch0, wint_t *ch1, wint_t *ch2, wint_t *ch3)
{
    wint_t t;
    // shift arrow keys
    if (ch0 == 49)
    {
        wchar_t seq[2];
        wchar_t chk[2] = {59, 50};
        for (uint8_t i = 0; i < 2; i++)
        {
            get_wch(&seq[i]);
            if (i == 0)
                *ch1 = seq[i];
            if (i == 1)
                *ch2 = seq[i];
            if (seq[i] != chk[i])
            {
                unget_wch(seq[i]);
                return -1;
            }
        }
        get_wch(&t);
        *ch3 = t;
        switch (t)
        {
        case 65:
            return kKey_shift_up;
        case 66:
            return kKey_shift_down;
        case 67:
            return kKey_shift_right;
        case 68:
            return kKey_shift_left;
        case 70:
            return kKey_shift_pagedown;
        case 72:
            return kKey_shift_pageup;

        default:
            unget_wch(t);
            return -1;
        }
    }
    return -1;
}

/**
 * 
 */
void in_move_cursor(int direction)
{
    buffer_st *buffer = buffer_get_current();
    row_st *row = buffer->cursor_y < buffer->num_rows ? &buffer->rows[buffer->cursor_y] : NULL;

    if (direction == ES.config.keymap.key_up)
    {
        if (buffer->cursor_y != 0)
        {
            buffer->cursor_y--;
        }
        else
        {
            if (buffer->row_offset > 0)
            {
                buffer->row_offset--;
            }
        }
    }

    if (direction == ES.config.keymap.key_down)
    {
        if (buffer->cursor_y != ES.edit_rows - 1)
        {
            buffer->cursor_y++;
        }
        else if (buffer->row_offset != (buffer->num_rows - buffer->cursor_y))
        {
            buffer->row_offset++;
        }
    }

    if (direction == ES.config.keymap.key_left)
    {
        if (buffer->cursor_x != 0)
        {
            buffer->cursor_x--;
            if (row->special[buffer->cursor_x] == '\t')
            {
                while (buffer->cursor_x > 0 && row->special[buffer->cursor_x] == '\t')
                {
                    buffer->cursor_x--;
                }
            }
        }
        else if (buffer->cursor_y > 0)
        {
            buffer->cursor_y--;
            if (buffer->cursor_y < buffer->num_rows)
            {
                buffer->cursor_x = buffer->rows[buffer->cursor_y].size;
            }
            else
            {
                buffer->cursor_x = 0;
            }
        }
    }

    if (direction == ES.config.keymap.key_right)
    {
        if (row && buffer->cursor_x < row->size)
        {
            if (row->special[buffer->cursor_x] == '\t')
            {
                while (buffer->cursor_x < row->size && row->special[buffer->cursor_x] == '\t')
                {
                    buffer->cursor_x++;
                }
            }
            else
            {
                buffer->cursor_x++;
            }
        }
        else if (row && buffer->cursor_x == row->size)
        {
            buffer->cursor_y++;
            buffer->cursor_x = 0;
        }
    }
}

/**
 * 
 */
void in_process_keypress()
{
    timeout(500);
    wint_t ch0 = 0, ch1 = 0, ch2 = 0, ch3 = 0, ch4 = 0;
    int r = get_wch(&ch0);

    if (r == ERR)
    {
        return;
    }

    bool is_handled = false;

    buffer_st *buffer = buffer_get_current();
    if (buffer == NULL)
    {
        term_die("buffer");
    };
    row_st *row = buffer->cursor_y < buffer->num_rows ? &buffer->rows[buffer->cursor_y] : NULL;
    keymap_st keymap = ES.config.keymap;

    if (ch0 == keymap.key_quit)
    {
        term_close();
        exit(0);
    }

    // arrow keys
    if (ch0 == keymap.key_up)
    {
        in_move_cursor(keymap.key_up);
        is_handled = true;
    }
    if (ch0 == keymap.key_down)
    {
        in_move_cursor(keymap.key_down);
        is_handled = true;
    }
    if (ch0 == keymap.key_left)
    {
        in_move_cursor(keymap.key_left);
        is_handled = true;
    }
    if (ch0 == keymap.key_right)
    {
        in_move_cursor(keymap.key_right);
        is_handled = true;
    }

    // page up - page down
    if (ch0 == keymap.key_pagedown || ch0 == keymap.key_pageup)
    {
        int times = ES.edit_rows;
        while (times--)
        {
            in_move_cursor(ch0 == keymap.key_pagedown ? keymap.key_down : keymap.key_up);
        }
        is_handled = true;
    }

    // home and end
    if (ch0 == 27)
    {
        get_wch(&ch1);
        if (ch1 == keymap.key_end[1] || ch1 == keymap.key_home[1])
        {
            get_wch(&ch2);
            if (ch2 == keymap.key_end[2])
            {
                buffer->cursor_x = row->size;
            }
            else if (ch2 == keymap.key_home[2])
            {
                buffer->cursor_x = 0;
            }
            is_handled = true;
        }
    }

    // delete
    if (ch0 == keymap.key_delete)
    {
        if (buffer->cursor_x == 0 && row->size == 0)
        {
            row_delete(buffer, buffer->cursor_y);
        }
        else
        {
            row_delete_wchar(row, buffer->cursor_x);
        }

        if (buffer->cursor_x == row->size && row->size > 0 && (buffer->cursor_y + 1) < buffer->num_rows)
        {
            row_join(buffer, buffer->cursor_y, buffer->cursor_y + 1);
            row_delete(buffer, buffer->cursor_y + 1);
        }
        is_handled = true;
    }

    if (ch0 == keymap.key_backspace)
    {
        if (buffer->cursor_x > 0)
        {
            row_delete_wchar(row, buffer->cursor_x - 1);
            buffer->cursor_x--;
        }
        else
        {
            if (row->size > 0 && buffer->cursor_y > 0)
            {
                uint32_t o_size = row_join(buffer, buffer->cursor_y - 1, buffer->cursor_y);
                row_delete(buffer, buffer->cursor_y);
                buffer->cursor_x = o_size;
                buffer->cursor_y--;
            }
            else
            {
                row_delete(buffer, buffer->cursor_y);
                in_move_cursor(keymap.key_left);
            }
        }
        is_handled = true;
    }

    // special sequences
    switch (_in_get_special_sequence(ch0, &ch1, &ch2, &ch3))
    {
    case kKey_shift_up:
        is_handled = true;
        break;

    case kKey_shift_down:
        is_handled = true;
        break;

    case kKey_shift_left:
        is_handled = true;
        break;

    case kKey_shift_right:
        is_handled = true;
        break;

    case kKey_shift_pagedown:
        is_handled = true;
        break;

    case kKey_shift_pageup:
        is_handled = true;
        break;
    }

    if ((iswalnum(ch0) || iswspace(ch0) || iswpunct(ch0)) && is_handled == false && row)
    {
        row_insert_wchar(row, buffer->row_offset, ch0);
        buffer->cursor_x++;
    }

    out_status_message(L"KEY COMBO: %d, %d, %d, %d | 0%o, 0%o, 0%o, 0%o, 0%o", ch0, ch1, ch2, ch3, ch4, ch0, ch1, ch2, ch3);

    // snap x to end of line
    row = (buffer->cursor_y < buffer->num_rows) ? &buffer->rows[buffer->cursor_y] : NULL;
    uint32_t eol = row ? row->size : 0;
    if (buffer->cursor_x > eol)
    {
        buffer->cursor_x = eol;
    }
}
