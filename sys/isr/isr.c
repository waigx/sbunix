/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015  Dongju Ok   <dongju@stonybrook.edu, yardbirds79@gmail.com>        
 *                      Yigong Wang <yigwang@cs.stonybrook.edu>
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
#include <stdio.h>
#include <sys/pic.h>

void divide_handler(uint64_t entry_num)
{
	printf("entry_num = %x\n", entry_num);
	
}

void timer_handler(void)
{
//	while(1);
//	volatile uint64_t count=100000;
	out_port_byte(0x20, 0x20);
	printf("I am in time_handler\n");
//	while(count--);
}
/*
void isr2(void)
{
        ;
}
void isr3(void)
{
        ;
}
void isr4(void)
{
        ;
}
void isr5(void)
{
        ;
}
void isr6(void)
{
        ;
}
void isr7(void)
{
        ;
}
void isr8(void)
{
        ;
}
void isr9(void)
{
        ;
}
void isr10(void)
{
        ;
}
void isr11(void)
{
        ;
}
void isr12(void)
{
        ;
}
void isr13(void)
{
        ;
}
void isr14(void)
{
        ;
}
void isr15(void)
{
        ;
}
void isr16(void)
{
        ;
}
void isr17(void)
{
        ;
}
void isr18(void)
{
        ;
}
void isr19(void)
{
        ;
}
void isr20(void)
{
        ;
}
void isr21(void)
{
        ;
}
void isr22(void)
{
        ;
}
void isr23(void)
{
        ;
}
void isr24(void)
{
        ;
}
void isr25(void)
{
        ;
}
void isr26(void)
{
        ;
}
void isr27(void)
{
        ;
}
void isr28(void)
{
        ;
}
void isr29(void)
{
        ;
}
void isr30(void)
{
        ;
}
void isr31(void)
{
        ;
}
void isr32(void)
{
        ;
}
void isr33(void)
{
        ;
}
void isr34(void)
{
        ;
}
void isr35(void)
{
        ;
}
void isr36(void)
{
        ;
}
void isr37(void)
{
        ;
}

*/











