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


#include <sys/sched/sched.h>
#include <sys/mem.h>
#include <sys/managemem.h>
#include <sys/elf.h>
#include <string.h>
#include <sys/debug.h>
#include <sys/sbunix.h>

void init_task(task_t *task, uint64_t entry_point, uint64_t *user_stack_base, uint64_t *kernel_stack_base);
void init_task_context(task_t *task, uint64_t entry_point, uint64_t *stack_base);

/* temp code */
struct tss_t tss_buf[16];
uint8_t kernel_stack[4096 * 2];

task_t *newtask(const char *task_name, process_type_t type)
{
	uint64_t entry_point;
	uint64_t *phy_stack_base;
	task_t *task = g_task_start + g_next_task_free_index;
	kpid_t new_pid = g_next_task_free_index;
	cr3e_t new_cr3;

	new_cr3 = newvmem(new_pid);

	task->cr3 = new_cr3;
	task->pid = new_pid;
	task->parent = gp_current_task->pid;
	task->type = type;
	task->status = PROCESS_NEW;
	strcpy(task->name, task_name);
	phy_stack_base = allocframe(new_pid);
	kmmap(pe2physaddr(new_cr3), new_pid, (uint64_t)phy_stack_base, USER_STACK_START);

	if(type == KERNEL_PROCESS) {
//		init_task(task, instruction_addr, virtual_memory_addr);
//		page = (uint64_t)allocframe(pid);
	} else if (type == USER_PROCESS) {
		/* KERNEL STACK Setting */
		phy_stack_base = allocframe(new_pid);
		kmmap(pe2physaddr(new_cr3), new_pid, (uint64_t)phy_stack_base, KERNEL_STACK_START);

		entry_point = load_elf(task, task_name);
		init_task(task, entry_point, (uint64_t *)USER_STACK_START, (uint64_t *)KERNEL_STACK_START);
		
		task->tsss = (struct tss_t *)&(tss_buf[task->pid]);
	}


	/*
	 * Allocate new pid
	 */
	while ((g_task_start + g_next_task_free_index)->pid != 0) {
		g_next_task_free_index += 1;
		if (g_next_task_free_index > g_task_bump)
			g_task_bump++;
		if (g_next_task_free_index >= MAX_PROC_NUM);
		// max process num exceed error here;
	}


	// add task list

	return task;
}


void init_task(task_t *task, uint64_t entry_point, uint64_t *user_stack_base, uint64_t *kernel_stack_base)
{
	task->u_stack_base = user_stack_base;
	task->k_stack_base = kernel_stack_base;
	init_task_context(task, entry_point, user_stack_base /*kernel_stack_base*/);
}


void init_task_context(task_t *task, uint64_t entry_point, uint64_t *stack_base )
{
	//setmem(task->context, (uint64_t*)((uint64_t)task->context + sizeof(struct regs_struct)), 0x00);
	setmem((uint64_t *)task->context.regs, (uint64_t *)task->context.regs + sizeof(struct regs_struct), 0x00);

	// TODO stack_base will be allocated size or memory menagement by dongju
	task->context.regs[CONTEXT_RSP_OFFSET] = (uint64_t)stack_base + 4096 -8;
	task->context.regs[CONTEXT_RBP_OFFSET] = (uint64_t)stack_base + 4096 -8;

#if(0)
	task->context.regs[CONTEXT_CS_OFFSET] = GDT_KERNEL_CODE_SEG;
	task->context.regs[CONTEXT_DS_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_ES_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_FS_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_GS_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_SS_OFFSET] = GDT_KERNEL_DATA_SEG;
#else
	task->context.regs[CONTEXT_CS_OFFSET] = GDT_USER_CODE_SEG;
	task->context.regs[CONTEXT_DS_OFFSET] = GDT_USER_DATA_SEG;
	task->context.regs[CONTEXT_ES_OFFSET] = GDT_USER_DATA_SEG;
	task->context.regs[CONTEXT_FS_OFFSET] = GDT_USER_DATA_SEG;
	task->context.regs[CONTEXT_GS_OFFSET] = GDT_USER_DATA_SEG;
	task->context.regs[CONTEXT_SS_OFFSET] = GDT_USER_DATA_SEG;

#endif


	task->context.regs[CONTEXT_RIP_OFFSET] = entry_point;
	// TODO needed to enable interrupt by dongju
	task->context.regs[CONTEXT_RFLAG_OFFSET] |= (0x1 << 9);	// IF(bit 9) enable	
}


