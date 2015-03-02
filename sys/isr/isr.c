/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
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


#include <sys/isr.h>
#include <sys/sbunix.h>
#include <sys/pic.h>
#include <sys/keyboard.h>

void divide_handler(uint64_t entry_num)
{
	printf("entry_num = %x\n", entry_num);
}

void timer_handler(void)
{
	printf("I am in time_handler\n");
	send_eoi(TIMER_IRQ_NUMBER);
}

void keyboard_handler(void)
{
        uint8_t ch = 0;
        ch = get_char_keyboard();
	printf("keyboard = %c\n", ch);
	send_eoi(KEYBOARD_IRQ_NUMBER);
}

