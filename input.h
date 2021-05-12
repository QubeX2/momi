/**
 * 
 */
#pragma once
#ifndef __INPUT_H__
#define __INPUT_H__

enum input_special_key_e {
    kKey_shift_right = 1000,
    kKey_shift_left,
    kKey_shift_up,
    kKey_shift_down,
    kKey_shift_pagedown,
    kKey_shift_pageup,
};

void in_process_keypress();
void in_move_cursor(int direction);

#endif /* __INPUT_H__ */