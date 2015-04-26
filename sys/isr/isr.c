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


#include <sys/isr.h>
#include <sys/sbunix.h>
#include <sys/pic.h>
#include <sys/kio.h>
#include <sys/keyboard.h>
#include <sys/managemem.h>
#include <sys/timer.h>
#include <sys/register.h>
#include <sys/debug.h>
#include <sys/sched/sched.h>


void divide_handler(uint64_t entry_num)
{
	printf("entry_num = %x\n", entry_num);
}


void debug(uint64_t err_code)
{
	printf("[IDT Fault Number:] %d\n", err_code);
	debug_pause();
}


void pagefault_handler(void)
{
	uint64_t vaddr = get_cr2_register();
	printf("[Page Fault:] %p\n", vaddr);
	newvaddr(gp_current_task->pid, vaddr);
	printf("[Returned]\n");
}


void timer_handler(void)
{
	echotime();
	 __asm volatile("cli");
	send_eoi(TIMER_IRQ_NUMBER);
	scheduler();
	//send_eoi(TIMER_IRQ_NUMBER);
}


void keyboard_handler(void)
{
	echokeyboard();
	send_eoi(KEYBOARD_IRQ_NUMBER);
}
