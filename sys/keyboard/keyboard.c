/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sbunix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sbunix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sbunix.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/pic.h>
#include <sys/keyboard.h>


#define KEYBOARD_CONTROL_REGISTER       0x64
#define KEYBOARD_STATUS_REGISTER        0x64
#define KEYBOARD_INPUT_BUFFER           0x60
#define KEYBOARD_OUTPUT_BUFFER          0x60

#define KEYBOARD_CONTROLLER_ENABLE_CMD  0xAE

#define KEYBOARD_ACTIVITY_CMD           0xF4
#define KEYBOARD_OK_RESPONSE            0xFA


uint64_t
init_keyboard(void)
{
	uint64_t i = 0;
	out_port_byte( KEYBOARD_CONTROL_REGISTER, KEYBOARD_CONTROLLER_ENABLE_CMD);

	for (i=0; i < 0xFFFF; i++)
	{
		if (is_input_buf_full() == FALSE)
			break;
	}

	out_port_byte(KEYBOARD_INPUT_BUFFER, KEYBOARD_ACTIVITY_CMD);

	for (i=0; i < 0xFFFF; i++)
	{
		if (is_output_buf_full() == TRUE)
			break;
	}
	if (in_port_byte(KEYBOARD_OUTPUT_BUFFER) == KEYBOARD_OK_RESPONSE)
	{
		return TRUE;
	}
	else
		return FALSE;

}


uint8_t get_scancode_keyboard(void)
{
	return in_port_byte(KEYBOARD_OUTPUT_BUFFER);
}


uint64_t
is_output_buf_full(void)
{
	if (in_port_byte(KEYBOARD_STATUS_REGISTER) & 0x01)
	{
		return TRUE;
	}
	else
		return FALSE;
}


uint64_t
is_input_buf_full(void)
{
	if (in_port_byte(KEYBOARD_STATUS_REGISTER) & 0x02)
	{
		return TRUE;
	}
	else
		return FALSE;
}

