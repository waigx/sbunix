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


#include <sys/general.h>
#include <sys/managemem.h>
#include <sys/sched/sched.h>
#include <sys/kio.h>
#include <sys/sbunix.h>
#include <sys/debug.h>


void _init_mem_pools()
{
	return;
}


cr3e_t _init_kernel_mem(kpid_t pid, uint64_t physbase, uint64_t physfree, uint64_t physbottom, uint64_t phystop)
{
	uint64_t i;
	uint64_t page_frame_start = (uint64_t)g_page_frame_start;
	pml4e_t *pml4e_p = (pml4e_t *)newmemtable(pid, 1 << VADDR_PML4E, TRUE);

	for (i = (uint64_t)0; i <= phystop; i+= (PAGE_SIZE))
		kmmap(pml4e_p, pid, i, i);

	for (i = (uint64_t)0; i < page_frame_start; i += (PAGE_SIZE))
		kmmap(pml4e_p, pid, i, i + KERNEL_SPACE_START);

	kmmap(pml4e_p, pid, CONSOLE_START, CONSOLE_START);
	
	printf("[Kernel Init Mem]: Kernel memory tables take %d page frames.\n", g_next_free_frame_index);

	return (cr3e_t)physaddr2pebase(pml4e_p);
}


void _init_kernel_process(void *physbase, void *physfree, void *physbottom, void *phystop)
{
	task_t *kproc = g_task_start + g_next_task_free_index;
	kpid_t kpid = g_next_task_free_index;
	cr3e_t kcr3;

	kcr3 = _init_kernel_mem(kpid, (uint64_t)physbase, (uint64_t)physfree, (uint64_t)physbottom, (uint64_t)phystop);

	kproc->cr3 = kcr3;
	kproc->pid = kpid;

	g_next_task_free_index += 1;
	return;
}


void init_kernel(void *physbase, void *physfree, void *physbottom, void *phystop)
{

	_init_mem_pools();

	g_physfree = physfree;
	g_physbase = physbase;
	g_page_frame_start = physfree + KERNEL_PROC_HEAP_SIZE + PAGE_SIZE;
	// align page table to PAGE_SIZE
	g_page_frame_start = (void *)(((uint64_t)g_page_frame_start) >> PAGE_SIZE_LOG2 << PAGE_SIZE_LOG2);
	g_task_start = (task_t *)g_physfree;

	_init_kernel_process(physbase, physfree, physbottom, phystop);
	loadtask(KERNEL_PID);
	debug_pause();
	return;
}
