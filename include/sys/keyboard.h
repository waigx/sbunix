#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <sys/defs.h>


uint64_t init_keyboard(void);
uint8_t get_char_keyboard(void);

uint64_t is_output_buf_full(void);
uint64_t is_input_buf_full(void);

#endif        
