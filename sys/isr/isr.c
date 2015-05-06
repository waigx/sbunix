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
#include <sys/sched.h>

uint64_t g_keyboard_buf;
uint64_t g_valid_keyboard= FALSE;
uint64_t  g_diable_scheduler = 0;


void divide_handler(uint64_t entry_num)
{
	printf("entry_num = %x\n", entry_num);
}


void debug(uint64_t err_code)
{
#if DEBUG_FATAL
	debug_print("ISR", "Fault number: %d\n", err_code);
	debug_print("ISR", "Intentional pause now.\n");
	debug_pause();
#endif
}


void pagefault_handler(void)
{
	uint64_t vaddr = get_cr2_register();
	uint64_t pte_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_OFFSET);
	vma_t *belonged_vma;
	pte_t pte;

#if DEBUG_PAGEFAULT
	debug_print("PagFlt", "Virtual addr: %p\n", vaddr);
#endif

	belonged_vma = lookupvmabyvaddr((void *)vaddr);

	if (belonged_vma == NULL) {
#if DEBUG_PAGEFAULT
		debug_showvmas(g_vma_start);
		debug_print("PagFlt", "No VMAs \n");
#endif
		printf("Segmentation Fault at address:%p\n", vaddr);
		sys_exit(-1);
		return;
	}

	if ((VMA_WRITEABLE & belonged_vma->permission) == FALSE) {
#if DEBUG_PAGEFAULT
		debug_showvmas(g_vma_start);
		debug_print("PagFlt", "VMAs Permission\n");
#endif
		printf("Segmentation Fault at address:%p\n", vaddr);
		sys_exit(-1);
		return;
	}

	pte = *(getptep(vaddr) + pte_offset);
	if ((pte & PTE_WRITEABLE) == FALSE && (pte & PTE_PRESENTS) == TRUE && g_page_frame_pool[physaddr2frameindex(pe2physaddr(pte))] > 0){
		cow_pagefault_handler(vaddr);
		return;
	}

	newvaddr(vaddr);
	return;
}


void timer_handler(void)
{
	 __asm volatile("cli");
	echotime();
	send_eoi(TIMER_IRQ_NUMBER);
	scheduler();
}


void keyboard_handler(void)
{
	g_keyboard_buf = echokeyboard();
	if(g_keyboard_buf != 0)
	{
		kb_queue[start_queue++] = g_keyboard_buf;
		if(start_queue > KEYBOARD_BUF_SIZE - 1)
			start_queue = 0;

		g_valid_keyboard = TRUE;
	}
	send_eoi(KEYBOARD_IRQ_NUMBER);
}
