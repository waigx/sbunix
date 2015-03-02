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


#ifndef _PIC_H
#define _PIC_H

#include <sys/defs.h>


#define PIC_MASTER_COMMAND     0x20
#define PIC_MASTER_DATA        0x21
#define PIC_SLAVE_COMMAND      0xA0
#define PIC_SLAVE_DATA         0xA1

#define ENABLE_TIMER_INT        0x01
#define ENABLE_KEYBOARD_INT     0x02

void init_pic(uint16_t enable_interrupt);
void send_eoi(uint16_t irq_num);


static __inline void out_port_byte(unsigned short port, unsigned char data)
{
	__asm__ __volatile__(
		"outb %0, %1"
		: 
		: "a" (data), "d" (port));

}

static __inline uint8_t in_port_byte(unsigned short  port)
{
	uint8_t r=0;
	__asm__ __volatile__(
	"inb %1, %0"
	:  "=a" (r)
	: "d" (port));

	return r;
}


#endif
