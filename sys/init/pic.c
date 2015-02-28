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


/* To set cascaded PIC environment
 * 0b 0001 0001
 */
#define ICW1 0x11

/* This will remap Interrupt Vector Table
 */
#define ICW2_MASTER 0x20
#define ICW2_SLAVE 0x28

/* To set the IRQ line for cascaded mode
 * Master PIC: 0b 0000 0100
 * Slave PIC: 0b 0000 0010
 */
#define ICW3_MASTER 0x04
#define ICW3_SLAVE 0x02

/* To enable 80x86 mode of PIC
 * 0b 0000 0001
 */ 
#define ICW4 0x01


/**
 * init_pic - init pic controller(Master and Slave)
 *
 * Returns value is void 
 *
 */
void init_pic()
{
	/* Set ICW1 */
	out_port_byte(PIC_MASTER_COMMAND, ICW1);
	out_port_byte(PIC_SLAVE_COMMAND, ICW1);

	/* This will remap Interrupt Vector Table */
	out_port_byte(PIC_MASTER_DATA, ICW2_MASTER);
	out_port_byte(PIC_SLAVE_DATA, ICW2_SLAVE);

	/* Set ICW3 */
	out_port_byte(PIC_MASTER_DATA, ICW3_MASTER);
	out_port_byte(PIC_SLAVE_DATA, ICW3_SLAVE);

	/* Set 80x86 mode */
	out_port_byte(PIC_MASTER_DATA, ICW4);
	out_port_byte(PIC_SLAVE_DATA, ICW4);
}



