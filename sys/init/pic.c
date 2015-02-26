/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 	Dongju Ok   <dongju@stonybrook.edu, yardbirds79@gmail.com>	  
 *			Yigong Wang <yigwang@cs.stonybrook.edu>
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
#include <stdio.h>

#define PIC_MASTER_PORT1	0x20
#define PIC_MASTER_PORT2	0x21
#define PIC_SLAVE_PORT1		0xA0
#define PIC_SLAVE_PORT2		0xA1

/**
 * init_pic - init pic controller(Master and Slave)
 *
 * Returns value is void 
 *
 * Note: * This function set PIC for interrupt ie, timer.
 *       * But, I cannot read this value, I don't know why
 */
void init_pic()
{
	out_port_byte(PIC_MASTER_PORT1, 0x11);
	out_port_byte(PIC_MASTER_PORT2, 0x20);
	out_port_byte(PIC_MASTER_PORT2, 0x04);
	out_port_byte(PIC_MASTER_PORT2, 0x01);
	out_port_byte(PIC_SLAVE_PORT1, 0x11);
	out_port_byte(PIC_SLAVE_PORT2, 0x20 + 8);
	out_port_byte(PIC_SLAVE_PORT2, 0x02);
	out_port_byte(PIC_SLAVE_PORT2, 0x01);
}


void out_port_byte(unsigned short port, unsigned char data)
{
	__asm volatile ("outb %0, %1"
			: 	// output 
			: "a" (data), "d" (port));

}

uint8_t in_port_byte(unsigned short  port)
{
	uint8_t r=0;
	__asm volatile("inb %1, %0"
			:  "=a" (r)   //output
			: "d" (port));
	return r;
}


